#ifndef AVL_H
#define AVL_H
#include <cstring>
#include <QDebug>
#include <QFile>
#include <iostream>
#include <QTextStream>
#include<iomanip>
#include <QQueue>
using namespace std;

template <typename U,typename P>
class AVL {
private:
    class AVLNode {
    public:
        U data;
        P _password;

        short int balanceFactor;
        AVLNode* right;
        AVLNode* left;
        AVLNode* parent;

        AVLNode() {}
        AVLNode(U item,P pass) :balanceFactor(0), data(item),_password(pass), left(0), right(0),parent(0) {}
    };


    typedef AVLNode* AVLNodePointer;

    int Height(AVLNodePointer Root);       //求树的高度
    void rotateL(AVLNodePointer& parent);   //单旋转  左 (传指针引用类型）
    void rotateR(AVLNodePointer& parent);   //单旋转  右 （传指针引用类型）

    void RotateL(AVLNodePointer parent);   //单旋转  左 (不传指针引用类型）
    void RotateR(AVLNodePointer parent);   //单旋转  右 （不传指针引用类型）
    void rotateRL(AVLNodePointer& parent);   //双旋转  右左
    void rotateLR(AVLNodePointer& parent);   //双旋转  左右


    void search2(const U& item, bool& found, AVLNodePointer& locptr, AVLNodePointer& parent) const;
    void inorderAux(ostream& out, AVLNodePointer subtreeRoot) const;
    void horizontal_graphAux(ostream& out, int indent, AVLNodePointer subtreeRoot) const;
    void xigouAux(AVLNodePointer subtreeRoot);


public:
    AVLNodePointer myRoot;  //根结点指针
    AVL();
    bool empty() const;                       //判断二叉查找树是否为空
    bool search(const U& item,const P& pass) const;  //通过账号密码查找
    bool _search(const U& item) const;               //通过账号查找
    bool modify(const U& item,const P& pass);        //修改密码
    bool add(const U& item,const P& pass);           //结点的添加
    void remove(const U& item);        //结点的删除
    void inorder(ostream& out) const;        //中序遍历
    void horizontal_gragh(ostream& out) const;          //图形输出---横着
    bool _IsBalance(AVLNodePointer Root, int& height);
    bool IsBalance(AVLNodePointer Root);
    void readTxt(QString path);//读取文件
    void level_OrderWrite(QString path);//层次遍历写入文件

    ~AVL();                                  //析构函数


};
//---------------------------------------------------------------------------读取文件
template <typename U, typename P>
void  AVL<U,P>::readTxt(QString path)
{
    QFile f(path);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Open failed." ;
        return ;
    }

    QTextStream txtInput(&f);
    QString lineAccount,linePassword;
    while(!txtInput.atEnd())
    {
        lineAccount = txtInput.readLine();
        linePassword = txtInput.readLine();
        this->add(lineAccount,linePassword);
    }

    f.close();
}

//---------------------------------------------------------------------------写文件
template <typename U,typename P>
void AVL<U,P>::level_OrderWrite(QString path)
{
    QFile f(path);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Open failed." << "\n";
        return ;
    }
    QTextStream txtOutput(&f);
    QQueue<AVL<U,P>::AVLNodePointer> q;
    q.clear();
    AVL<U,P>::AVLNodePointer p=myRoot;
    while(p){
        txtOutput<<p->data<<"\n";
        txtOutput<<p->_password<<"\n";
        if(p->left)
            q.enqueue(p->left);
        if(p->right)
            q.enqueue(p->right);
        if(q.isEmpty()){
            return;
        }
        p=q.head();
        q.pop_front();
    }
    f.close();
}

//---------------------------------------------------------------------------插入
template <typename U, typename P>
bool AVL<U,P>::add(const U& item,const P& pass)
{
    if (myRoot == NULL) //树为空
    {
        AVL<U, P>::AVLNodePointer temp = new AVL<U,P>::AVLNode(item,pass);
        myRoot = temp;
        return true;
    }
    else
    {
        AVL<U, P>::AVLNodePointer temp = new AVL<U,P>::AVLNode(item,pass);
        AVL<U, P>::AVLNodePointer cur = myRoot;
        AVL<U, P>::AVLNodePointer parent = NULL;
        while (cur)
        {
            if (item > cur->data)
            {
                parent = cur;
                cur = cur->right;
            }
            else if (item < cur->data)
            {
                parent = cur;
                cur = cur->left;
            }
            else
                return false;
        }
        //此处cur==NULL找到插入结点
        cur = temp;
        if (parent->data < cur->data)
        {
            //插入结点在右边
            parent->right = cur;
        }
        else
        {
            //插入结点在左边
            parent->left = cur;
        }
        cur->parent = parent;
        while (parent)
        {
            if (parent->left == cur)
                parent->balanceFactor++;
            if (parent->right == cur)
                parent->balanceFactor--;
            if (parent->balanceFactor == 0)
                break;
            else if ((parent->balanceFactor == 1) || (parent->balanceFactor == -1))
            {
                cur = parent;
                parent = cur->parent;
            }
            else //平衡因子变为2或者-2
            {
                int bf = parent->balanceFactor < 0 ? -1 : 1;
                //parent平衡因子和cur平衡因子相同只可能为单旋转
                if (bf == cur->balanceFactor)
                {
                    if (bf == 1) //加入结点在左边需要右单旋
                        rotateR(parent);
                    else
                        rotateL(parent);
                }
                else
                {
                    if (bf == 1)
                        rotateLR(parent);
                    else
                        rotateRL(parent);
                }
                break;
            }
        }

    }
    return true;
}
//------------------------------------------------------------------------简单左旋
template <typename U, typename P>
void AVL<U,P>::rotateL(AVL<U,P>::AVLNodePointer& parent)
{

    AVL<U, P>::AVLNodePointer subR = parent->right;
    AVL<U, P>::AVLNodePointer subRL = subR->left;
    parent->right = subRL;
    if (subRL)
        subRL->parent = parent;

    subR->left = parent;

    AVL<U, P>::AVLNodePointer pparent = parent->parent;
    parent->parent = subR;
    if (pparent)
    {
        if (pparent->left == parent)
            pparent->left = subR;
        else
            pparent->right = subR;
        subR->parent = pparent;
    }
    else
    {
        myRoot = subR;
        subR->parent = NULL;
    }
    subR->balanceFactor = 0;
    parent->balanceFactor = 0;
    parent = subR;
}
//不传指针引用
template <typename U, typename P>
void AVL<U, P>::RotateL(AVL<U, P>::AVLNodePointer parent)
{

    AVL<U, P>::AVLNodePointer subR = parent->right;
    AVL<U, P>::AVLNodePointer subRL = subR->left;
    parent->right = subRL;
    if (subRL)
        subRL->parent = parent;

    subR->left = parent;

    AVL<U, P>::AVLNodePointer pparent = parent->parent;
    parent->parent = subR;
    if (pparent)
    {
        if (pparent->left == parent)
            pparent->left = subR;
        else
            pparent->right = subR;
        subR->parent = pparent;
    }
    else
    {
        myRoot = subR;
        subR->parent = NULL;
    }
    subR->balanceFactor = 0;
    parent->balanceFactor = 0;
}
//--------------------------------------------------------------------------简单右旋
template <typename U, typename P>
void AVL<U,P>::rotateR(AVL<U,P>::AVLNodePointer& parent)
{
    AVL<U, P>::AVLNodePointer subL = parent->left;
    AVL<U, P>::AVLNodePointer subLR = subL->right;
    parent->left = subLR;
    if (subLR)
        subLR->parent = parent;
    subL->right = parent;

    AVL<U, P>::AVLNodePointer pparent = parent->parent;
    parent->parent = subL;
    if (pparent)
    {
        if (pparent->left == parent)
            pparent->left = subL;
        else
            pparent->right = subL;
        subL->parent = pparent;
    }
    else
    {
        myRoot = subL;
        subL->parent = NULL;
    }
    subL->balanceFactor = 0;
    parent->balanceFactor = 0;
    parent = subL;
}
//不传指针引用

template <typename U, typename P>
void AVL<U, P>::RotateR(AVL<U, P>::AVLNodePointer parent)
{
    AVL<U, P>::AVLNodePointer subL = parent->left;
    AVL<U, P>::AVLNodePointer subLR = subL->right;
    parent->left = subLR;
    if (subLR)
        subLR->parent = parent;
    subL->right = parent;

    AVL<U, P>::AVLNodePointer pparent = parent->parent;
    parent->parent = subL;
    if (pparent)
    {
        if (pparent->left == parent)
            pparent->left = subL;
        else
            pparent->right = subL;
        subL->parent = pparent;
    }
    else
    {
        myRoot = subL;
        subL->parent = NULL;
    }
    subL->balanceFactor = 0;
    parent->balanceFactor = 0;
}
//--------------------------------------------------------------------------双旋转 先右转后左转
template <typename U, typename P>
void AVL<U,P>::rotateRL(AVL<U, P>::AVLNodePointer& parent)
{
    AVL<U, P>::AVLNodePointer subR = parent->right;
    AVL<U, P>::AVLNodePointer subRL = subR->left;

    int bf = subRL->balanceFactor;

    RotateR(parent->right);
    RotateL(parent);

    //平衡因子修正
    if (bf == 0)//这种情况subRL是新增结点
        parent->balanceFactor = subR->balanceFactor = 0;
    else if (bf == -1)//subRL的右边为新增结点
    {
        subR->balanceFactor = 0;
        parent->balanceFactor = 1;
    }
    else if (bf == 1)//subRL的左边为新增结点
    {
        subR->balanceFactor = -1;
        parent->balanceFactor = 0;
    }
    subRL->balanceFactor = 0;//最后更新平衡因子
}
//---------------------------------------------------------------------------双旋转 先左转后右转
template <typename U, typename P>
void AVL<U, P>::rotateLR(AVL<U, P>::AVLNodePointer& parent)
{
    AVL<U, P>::AVLNodePointer subL = parent->left;
    AVL<U, P>::AVLNodePointer subLR = subL->right;

    int bf = subLR->balanceFactor;

    RotateL(parent->left);
    RotateR(parent);

    if (bf == 0)//这种情况subLR是新增结点
        parent->balanceFactor = subL->balanceFactor = 0;
    else if (bf == -1)//subLR的右边为新增结点
    {
        subL->balanceFactor = 1;
        parent->balanceFactor = 0;
    }
    else if (bf == 1)//subLR的左边为新增结点
    {
        subL->balanceFactor = 0;
        parent->balanceFactor = -1;
    }
    subLR->balanceFactor = 0;//最后更新平衡因子

}
//---------------------------------------------------------------------构造函数
template <typename U, typename P>
inline AVL<U, P>::AVL() :myRoot(0) {}

//-----------------------------------------------------------------------析构函数
template <typename U, typename P>
inline AVL<U, P>::~AVL()
{
    xigouAux(myRoot);
}
template <typename U, typename P>
void AVL<U,P>::xigouAux(AVL<U, P>::AVLNodePointer subtreeRoot)
{
    if (subtreeRoot == NULL) return;
    AVLNodePointer  pLeft = subtreeRoot->left;
    AVLNodePointer  pRight = subtreeRoot->right;
    delete subtreeRoot;
    subtreeRoot = NULL;
    if (pLeft)  xigouAux(pLeft);
    if (pRight) xigouAux(pRight);
    return;
}
//------------------------------------------------------------------------求树高
template <typename U, typename P>
int AVL<U, P>::Height(AVL<U, P>::AVLNodePointer Root)
{
    if (Root == NULL)
        return 0;
    int left = Height(Root->left);
    int right = Height(Root->right);
    return left > right ? left + 1 : right + 1;
}
//------------------------------------------------------------------------判断树是否平衡
template <typename U, typename P>
bool AVL<U, P>::IsBalance(AVL<U, P>::AVLNodePointer Root)
{
    if (Root == NULL)
        return false;
    int left = Height(Root->left);
    int right = Height(Root->right);
    return abs(left - right) < 2
        && IsBalance(Root->left)
        && IsBalance(Root->right);

}
//-----------------------------------------------------------------------递归调用判断是否平衡，避免重复计算高度，直接遍历所有节点
template <typename U, typename P>
bool AVL<U, P>::_IsBalance(AVL<U, P>::AVLNodePointer Root,int& height)
{
    if (Root == NULL)
    {
        height = 0;
        return true;
    }
    int leftHeight = 0;
    if (_IsBalance(Root->left, leftHeight) == false)
        return false;
    int rightHeight = 0;
    if (_IsBalance(Root->right, rightHeight) == false)
        return false;
    height = leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;

    return abs(leftHeight - rightHeight) < 2;
}
//------------------------------------------------------------------------判空函数
template <typename U, typename P>
inline bool AVL<U, P>::empty()const
{
    return myRoot == 0;
}

////----------------------------------------------------------------------中序遍历操作
template <typename U, typename P>
inline void AVL<U, P>::inorder(ostream& out) const
{
    inorderAux(out, myRoot);//递归
}

template <typename U, typename P>
void AVL<U, P>::inorderAux(ostream& out,
    AVL<U, P>::AVLNodePointer subtreeRoot) const
{
    if (subtreeRoot != 0) {
        inorderAux(out, subtreeRoot->left);   //left操作
        out << subtreeRoot->data << " ";         //V操作
        inorderAux(out, subtreeRoot->right);  //right操作
    }
}

//--------------------------------------------------------------------横着的图形输出操作
template <typename U, typename P>
inline void AVL<U,P>::horizontal_gragh(ostream& out) const { horizontal_graphAux(out, 0, myRoot); }

template <typename U, typename P>
void  AVL<U, P>::horizontal_graphAux(ostream& out,	int indent,
    AVL<U, P>::AVLNodePointer subtreeRoot) const
{
    if (subtreeRoot != 0)
    {
        horizontal_graphAux(out, indent + 8, subtreeRoot->right);
        out << setw(indent) << " " << subtreeRoot->data << endl;
        horizontal_graphAux(out, indent + 8, subtreeRoot->left);
    }
}
//----------------------------------------------------------------------------查找操作
template <typename U, typename P>
bool AVL<U, P>::search(const U& item,const P& pass) const
{
    AVL<U, P>::AVLNodePointer locptr = myRoot;
    bool found = false;
    while (!found && locptr != 0)
    {
        if (item < locptr->data) locptr = locptr->left;
        else if (locptr->data < item) locptr = locptr->right;
        else
        {
            if(locptr->_password==pass)
                                found=true;
            else return false;
        }
    }
    return found;
}
template <typename U, typename P>
 bool  AVL<U, P>::_search(const U& item) const
 {
     AVL<U, P>::AVLNodePointer locptr = myRoot;
     bool found = false;
     while (!found && locptr != 0)
     {
         if (item < locptr->data) locptr = locptr->left;
         else if (locptr->data < item) locptr = locptr->right;
         else   found=true;
     }
     return found;
 }
 //--------------------------------------------------------------------修改密码
 template <typename U, typename P>
 bool AVL<U, P>::modify(const U& item,const P& pass)
 {
     AVL<U, P>::AVLNodePointer locptr = myRoot;
     bool found = false;
     while (!found && locptr != 0)
     {
         if (item < locptr->data) locptr = locptr->left;
         else if (locptr->data < item) locptr = locptr->right;
         else
         {
             locptr->_password=pass;
             found=true;
         }
     }
     return found;
 }

//-----------------------------------------------------------------------删除操作
template <typename U, typename P>
void AVL<U, P>::remove(const U& item)
{
    bool found;
    AVL<U, P>::AVLNodePointer
        x,                   //指向删除结点
        parent;              //指向x和xSucc的双亲节点
    search2(item, found, x, parent);

    if (!found)
    {
        cout << "Item not in the BST\n";
        return;
    }
    AVL<U, P>::AVLNodePointer ordinary_parent = parent;
    if ((x->left != 0 && x->right != 0)) //结点有两个子女 ,把它转换成一个子女情况
    {
        AVL<U, P>::AVLNodePointer xSucc = x->right;
        parent = x;
        while (xSucc->left != 0)
        {
            parent = xSucc;
            xSucc = xSucc->left;
        }
        //将xSucc的内容移到x中，修改x为指向将被删除的后继
        x->data = xSucc->data;
        x = xSucc;
    }//结点有两个子女情况处理结束

    //现在处理结点没有子女或者只有1个子女的情况
    AVL<U, P>::AVLNodePointer
        subtree = x->left;         //指向x的子树的指针
    if (subtree == 0)
        subtree = x->right;
    if (parent == 0)
        myRoot = subtree;
    else if (parent->left == x)      //删除结点在父节点左边
    {
        parent->left = subtree;
        if(subtree)
             subtree->parent = parent;
        parent->balanceFactor--;
    }

    else                              //删除结点在父节点右边
    {
        parent->right = subtree;
        if(subtree)
             subtree->parent = parent;
        parent->balanceFactor++;
    }

    delete x;  //完成删除

//现在进行删除后平衡因子的更新，平衡因子有三种情况

    AVL<U, P>::AVLNodePointer ptr;     //ptr为parent较高子树的头个结点
    while (parent)
    {
        if (parent->left == subtree)
            parent->balanceFactor--;
        else
            parent->balanceFactor++;

        if (parent->balanceFactor == 1 || parent->balanceFactor == -1)  //第一种情况，原本平衡，删完变成亚平衡，跳出
            break;

        if (parent->balanceFactor != 0)      //第二种情况，让原本亚平衡态更加不平衡了，被删除结点的父节点的更新后平衡因子为+2或-2，这时候有四种情况
                                             //左左，右右，左右，右左
        {
            if (parent->balanceFactor < 0)  //-2
            {
                ptr = parent->right;     //parent右边高，ptr标记为parent的右子头结点

                if (ptr->balanceFactor == 0)
                {
                    rotateL(parent);
                    parent->balanceFactor = 1;
                    (parent->left)->balanceFactor = -1;
                    break;                         //这种情况完成，直接跳出

                }
                else if (ptr->balanceFactor == -1)  //右右，进行左旋转
                    rotateL(parent);
                else                              //parent和ptr的平衡因子异号，右左折线
                    rotateRL(parent);
            }

            else              //这种情况就是镜像了，会前面就会这个
            {
                ptr = parent->left;

                if (ptr->balanceFactor == 0)
                {
                    rotateR(parent);
                    parent->balanceFactor = -1;
                    (parent->right)->balanceFactor = 1;
                    break;

                }
                else if (ptr->balanceFactor == 1)  //左左直线，进行右单旋
                    rotateR(parent);
                else
                    rotateLR(parent);
            }

            subtree = parent;
            parent = parent->parent;


        }
        else                             //第三种情况，删后父亲结点的平衡因子变为0，该树高-1，直接往前遍历检查
        {
            subtree = parent;
            parent = parent->parent;
        }

    }

}

template <typename U, typename P>
void AVL<U, P> ::search2(const U& item,
    bool& found, AVL<U, P>::AVLNodePointer& locptr,
    AVL< U, P>::AVLNodePointer& parent) const
{
    locptr = myRoot;
    parent = 0;
    found = false;
    while (!found && locptr != 0)
    {
        if (item < locptr->data)   //下降到左子树
        {
            parent = locptr;
            locptr = locptr->left;
        }
        else if (locptr->data < item)  //下降到右子树
        {
            parent = locptr;
            locptr = locptr->right;
        }
        else
            found = true;

    }
}
#endif // AVL_H
