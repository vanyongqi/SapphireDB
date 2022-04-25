#include<iostream>
using namespace std;
typedef enum
{
    EN_DRAW_SQUARE = 0,
    EN_DRAW_TRIANGLE,
}ENUM_DRAW_TYPE;

#define  SAFE_DELETE_PTR(p) if ((p) != NULL)\
{            \
    delete p;\
    p = NULL;\
}
//图形基类
class CGraph
{
public:
    CGraph(){}
    virtual ~CGraph(){}
public:
    virtual void Draw() = 0;
    virtual void Erase() = 0;
};
//正方形
class CSquare:public CGraph
{
public:
    CSquare():m_width(10){ std::cout << 
    "完成正方形对象创建" << endl;}
    ~CSquare(){}
public:
    virtual void Draw()
    {
        std::cout << "绘制正方形" << endl;
    }
    virtual void Erase()
    {
        std::cout << "清除绘正方形" << endl;
    }
private:
    int m_width;//正方形的独有参数
};

//三角形
class CTriangle:public CGraph
{
public:
    CTriangle():m_bottom(10),m_high(12){std::cout << "完成三角形对象创建" << endl;}
    ~CTriangle(){}
public:
    virtual void Draw()
    {
        std::cout << "绘制三角形" <<endl;
    }
    virtual void Erase()
    {
        std::cout << "清除三角形" << endl;
    }

private:
    int m_bottom; //底
    int m_high;   //高
};

//图形创建工厂
class CGraphFactory
{
public:
    //根据参数创建不同的对象
    static CGraph * CreateGraph(ENUM_DRAW_TYPE type)
    {
        CGraph * pGraph = NULL;
        switch(type)
        {
        case EN_DRAW_SQUARE:
            {
                pGraph = new CSquare();
                break;
            }
        case EN_DRAW_TRIANGLE:
            {
                pGraph = new CTriangle();
                break;
            }
        default:
            break;
        }
        return pGraph;
    }
};


int main(){
 CGraph *pGraph = NULL;
std::cout<<"请求创建三角形:"<< endl;
pGraph  = CGraphFactory::CreateGraph(EN_DRAW_TRIANGLE);

 if (pGraph != NULL)
 {
     pGraph->Draw();
     pGraph->Erase();
     SAFE_DELETE_PTR(pGraph);
 }

 cout<< endl <<"请求创建正方形:"<< endl;
 pGraph = CGraphFactory::CreateGraph(EN_DRAW_SQUARE);
 if (pGraph != NULL)
 {
     pGraph->Draw();
     pGraph->Erase();
     SAFE_DELETE_PTR(pGraph);
 }
}