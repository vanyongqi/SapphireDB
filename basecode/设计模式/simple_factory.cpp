#include<iostream>
#include<string>
using namespace std;
/*

    e_book =0,
    e_phone,
    e_pad,
    e_watch,
    e_computer
*/
typedef enum e_type{
   create_e_book =0,
    create_e_phone,
    create_e_pad,
    create_e_watch,
    create_e_computer
}e_type;

#define DELETE_PTR(p) if((p)!= nullptr)\
{\
    delete p;\
    p = nullptr;\
}\

// base class
class e_product{
    public:
        e_product(){}
         virtual ~e_product(){}
        
    private:
        e_product(const e_product&) = delete;
        void operator = (const e_product&) =delete;
       
    public:
        virtual void start_product() = 0;
        virtual void end_product() = 0;
};

class e_book:public e_product{
    public:
        e_book():device("kindle paperwhite 5"){ 
            std::cout<<device<<std::endl;}
        ~e_book(){}
    public:
        virtual void start_product(){
            std::cout<<"设备开机，阅读《人类简史》，"<<std::endl;
        }
        virtual void end_product(){
            std::cout<<"读累了，设备关机。"<<std::endl;
        }
  
    private:
        string device;
};
class e_phone:public e_product{
    public:
        e_phone():device("iphone 4s"){
            std::cout<<device<<std::endl;
        }
        ~e_phone(){}
    public:
        virtual void start_product(){
            std::cout<<"设备开启，打电话给乔布斯"<<std::endl;
        }
        virtual void end_product(){
            std::cout<<"跟乔帮主聊的很der，设备关闭。"<<std::endl;
        }
    private:
        string device;
};
class product_Factory{
    public:
        static e_product * create_product(e_type product_type){
            e_product * product_object = nullptr;
            switch(product_type){
                case create_e_book :{
                    product_object = new e_book();
                    break;
                }
                case create_e_phone:{
                    product_object = new e_phone();
                    break;
                }
                default:
                break;
            }
            return product_object;
        }

};

int main(){
    e_product *product = nullptr;
    std::cout<<" 想看电子书了，意念创建一个函数请求 工厂帮我 创建 电子书"<<std::endl;
    product = product_Factory::create_product(create_e_book);
    if(product){
        product->start_product();
        product->end_product();
        DELETE_PTR(product);
    }

      std::cout<<" 想跟乔布斯骂一下cook的刀法精准，意念创建一个函数请求 工厂帮我 创建 手机"<<std::endl;
    product = product_Factory::create_product(create_e_phone);
    if(product){
        product->start_product();
        product->end_product();
        DELETE_PTR(product);
    }
}