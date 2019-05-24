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
#include <cstdlib>

using namespace std;
 
struct argmS
{
    bool flag1;
    mqd_t mq; 
};
 
void* funcThread(void* args)
{
    argmS* x = (argmS*)args;
    const char buf[10] = "LETI !" ;

    cout << "\nPutting a message into a queue:" << endl; 
    while (!x->flag1)
    {
	if (mq_send(x->mq, buf, 10*sizeof(char), 0)!=-1)
		 cout << buf << endl;
	else 
        perror("mq_send");
	    sleep(1);
    }

    cout << endl << "Thread has finished" << endl;

    pthread_exit((void*)333);
}
 
int main()
{
    argmS arg;
    arg.flag1 = false;

    mq_attr attr; //identifier attributes   
    
    attr.mq_flags = 0; //flag - 0 or NON_BLOCK
    attr.mq_maxmsg = 10; //max messages on queue 
    attr.mq_msgsize = 15*sizeof(char); //max size of message (byte)
    attr.mq_curmsgs = 0; // current message

    arg.mq = mq_open("/lachannel", O_CREAT|O_WRONLY|O_NONBLOCK, 0777, &attr);  
    if(arg.mq==-1)
    perror("mq_open"); 

    mq_getattr(arg.mq, &attr);
     
    pthread_t id;
    pthread_create(&id, NULL, funcThread, &arg);
    cout << "Thread has been created" << endl;
    getchar();

    arg.flag1 = true;
    int retval;

    pthread_join(id, NULL);
    cout << "Return code is " << retval << endl;

    cout << "Flag: " << attr.mq_flags << endl;
    cout << "Max-message: " << attr.mq_maxmsg << endl;
    cout << "Size message: " << attr.mq_msgsize << endl;
    cout << "Current message: " << attr.mq_curmsgs << endl;
    
    mq_close(arg.mq);
    mq_unlink("/lachannel");

    cout << "The program has finished sucessfully!" << endl;
    return 0;
}
