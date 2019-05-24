#include <fcntl.h>      
#include <iostream>        
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <sys/mman.h>     
#include <semaphore.h>
#include <signal.h>
#include <mqueue.h>
#include <cerrno>
using namespace std;
 
struct argmS
{
    bool flag1;
    mqd_t mq;
};


void* func(void* args)
{
    argmS* x=(argmS*)args;
    char buf[10];

    cout << "Retrieving message from queue:" << endl; //Извлечение сообщения из очереди
    while(!x->flag1)
    {
	if(mq_receive(x->mq, buf, 20*sizeof(char), 0)!=-1)
		cout << buf << endl;
	else perror("mq_receive");
	sleep(1);
    }
    cout << endl << "Thread has finished" << endl;
    pthread_exit((void*)333);
}
 
int main()
{
    argmS arg;
    arg.flag1 = false;

    mq_attr attr;
     mqd_t mqd;    
    
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 15*sizeof(char);
    attr.mq_curmsgs = 0;

    arg.mq = mq_open("/lachannel", O_CREAT|O_RDONLY|O_NONBLOCK, 0644, &attr);
    if(arg.mq==-1) 
        perror("mq_open");   


    pthread_t id;
    pthread_create(&id, NULL, func, &arg);

    cout << "Thread has been created" << endl;
    getchar();

    arg.flag1 = true;
    int retval;

    pthread_join(id, NULL);
    cout<<"Return code is " << retval << endl;

    mq_close(arg.mq);
    mq_unlink("/lachannel");
    
    cout << "The program has finished sucessfully!" << endl;
    return 0;
}
