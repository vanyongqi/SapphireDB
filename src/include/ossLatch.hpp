
/*******************************************************************************

ossLatch.hpp
pthread_mutex_t是锁
1.互斥锁
      用来保证临界资源被互斥访问。
2.读写锁
      在一个线程中，当一个读写锁中的读锁被申请后，其他线程可以再申请读锁，但不能申请写锁。
      在一个线程中，当一个读写锁中的写锁被申请后，其他线程不能申请读锁，也不能申请写锁。
3.自旋锁
      等待条件时，不让出cpu。nginx中由于每个工作进程要求快速响应用户，就用到了自选锁。
4.递归锁
      同一个线程可以对一个递归锁加锁多次（其他锁没有这样的功能），但必须解锁同样多次。
另外，主要通过条件变量和信号量来进行线程间同步

数据库是用线程池的框架，本身要求有线程切换，所以不必用自旋锁来提高效
系统调用用宏定义取别名，体现了oss层对平台相关系统调用的封装的思想
互斥锁和共享锁分别是对操作系统中的互斥锁和读写锁的简单封装
*******************************************************************************/

#ifndef OSSLATCH_HPP__
#define OSSLATCH_HPP__
#include <pthread.h>
enum OSS_LATCH_MODE{
   SHARED ,//共享锁
   EXCLUSIVE//独占锁
} ;

class ossXLatch//互斥锁 mutex{
private :
   pthread_mutex_t _lock ;
public :
   ossXLatch ()
   {
      pthread_mutex_init ( &_lock, 0 ) ;
   }
   ~ossXLatch ()
   {
      pthread_mutex_destroy ( &_lock ) ;
   }
   void get ()
   {
      pthread_mutex_lock ( &_lock ) ;
   }
   void release ()
   {
      pthread_mutex_unlock ( &_lock ) ;
   }
   bool try_get ()//
   {
      return ( pthread_mutex_trylock ( &_lock ) == 0 ) ;
   }
} ;

class ossSLatch//共享锁 shares lock read& write lock
{
private :
   pthread_rwlock_t _lock ;
public :
   ossSLatch ()
   {
      pthread_rwlock_init ( &_lock, 0 ) ;
   }

   ~ossSLatch ()
   {
      pthread_rwlock_destroy ( &_lock ) ;
   }

   void get ()
   {
      pthread_rwlock_wrlock ( &_lock ) ;
   }

   void release ()
   {
      pthread_rwlock_unlock ( &_lock ) ;
   }

   bool try_get ()
   {
      return ( pthread_rwlock_trywrlock ( &_lock ) == 0 ) ;
   }

   void get_shared ()//读锁 共享锁
   {
      pthread_rwlock_rdlock ( &_lock ) ;
   }

   void release_shared ()
   {
      pthread_rwlock_unlock ( &_lock ) ;
   }

   bool try_get_shared ()
   {
      return ( pthread_rwlock_tryrdlock ( &_lock ) == 0 ) ;
   }
} ;

#endif

