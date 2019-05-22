# OS_9_2019
# INTERACTION OF PROCESSES - MESSAGE QUEUE  
## Mục đích bài lab:  
Làm quen với công cụ tương tác các tiến trình - message queue (hàng đợi tin nhắn) và các lệnh hệ thống, đảm bảo cho việc tạo (create), đóng (close), xóa (unlink) hàng đợi, cũng như truyền và nhận tin nhắn.  
## Lý thuyết  
Hàng đợi tin nhắn (Message queue) - là công cụ, được cung cấp bởi tiến trình để tương tác. Hàng đợi tin nhắn chứa cơ chế đồng bộ hóa bên trong, đảm bảo không có cơ hội đọc tin nhắn từ hàng đợi rỗng và ghi vào hàng đợi đầy.  
Do thực tế, khi tạo (create) và mở (open) hàng đợi tin nhắn, truyền vào cho nó "name" - chuỗi ký tự, 2 tiến trình có thể có cùng một con trỏ đến cùng 1 hàng đợi.  
Trong hệ thống, hàng đợi tin nhắn được thực hiện như một dạng file đặc biệt, lifetime của nó không bị giới hạn bởi lifetime của tiến trình tạo ra nó.  
Tồn tại vài dạng giao diện chương trình (interfaces) để tạo hàng đợi tin nhắn.  
**1. Giao diện chương trình interface POSIX.**   
Hàng đợi tin nhắn được tạo bởi lệnh:  
````
mqd_t mq_open(const char *name,int oflag,mode_t mode,struct mq_attr *attr),
````
trong đó :  
`name` – tên của hàng đợi;  
`oflag` - flag quản lý các hành động (operations) tạo nên hàng đợi, khi tạo hàng đợi cần chỉ ra cờ O_CREAT;    
`mode` – quyền truy cập vào hàng đợi tin nhắn;   
`attr` – tham số (parameter) của hàng đợi tin nhắn.  

Tin nhắn được xếp hàng (is queued) bằng lệnh:  
````
int mq_send(mqd_t mqdes,const char *msg_ptr,size_t msg_len,unsigned msg_prio),
````
trong đó:  
`mqdes` -  định danh (identifier) của hàng đợi tin nhắn;  
`msg_ptr` – con trỏ đến tin nhắn;  
`msg_len` – chiều dài của tin nhắn;  
`msg_prio` – sự ưu tiên của tin nhắn. 

Tin nhắn được lấy ra (is retrieved) từ hàng đợi tin nhắn bằng lệnh:   
````
ssize_t mq_receive(mqd_t mqdes,char *msg_ptr,size_t msg_len,unsigned *msg_prio),
````
trong đó:  
`mqdes` -  định danh (identifier) của hàng đợi tin nhắn;  
`msg_ptr` – con trỏ đến bộ đệm (buffer) để nhận tin nhắn;  
`msg_len` – kích cỡ buffer;  
`msg_prio` – sự ưu tiên của tin nhắn.    

Hàng đợi tin nhắn được đóng (close) bằng lệnh:  
````
int mq_close(mqd_t mqdes).
````

Hàng đợi tin nhắn bị xóa (delete) bằng lệnh:  
````
int mq_unlink(const char *name).
````

**2. Giao diện chương trình SVID**  
Hàng đợi tin nhắn được tạo bởi lệnh:  
````
int msgget(key_t key, int msgflg);
````
trong đó :  
`key_t key` – khóa (key) được nhận từ hàm `ftok()`;  
`int msgflg` – cờ (flag), một cờ xác định các quyền để thực hiện các hoạt động, một giá trị cờ điển hình là giá trị IPC_CREAT | 0644.  
Sau khi tạo hàng đợi, việc truyền tin nhắn tồn tại dưới dạng:  
````
int msgsnd(int msqid, struct msgbuf *msgp, size_t msgsz, int msgflg);
````
где:  
`int msqid` – định danh (identifier) của hàng đợi;    
````
struct msgbuf *msgp – сообщение, сформированное в структуре:  
struct msgbuf {
long	mtype;	/* тип сообщения, должен быть > 0 */
char	mtext[1];	/* содержание сообщения */
};
````
`size_t msgsz` – kích thước tin nhắn;  
`int msgflg` – flags, mô tả chế độ làm việc của hàm, ví dụ, hàm gọi ko chặn (IPC_NOWAIT).  

Nhận tin nhắn từ hàng đợi bằng lệnh:  
````
ssize_t msgrcv(int msqid, struct msgbuf *msgp, size_t msgsz, long msgtyp, int msgflg);
````
trong đó:  
`int msqid` – định danh hàng đợi;  
`struct msgbuf *msgp` – bộ đệm để nhận tin nhắn;  
`size_t msgsz` – kích thước tin nhắn;  
`long msgtyp` – loại tin nhắn;  
`int msgflg` – flags, mô tả chế độ làm việc của hàm.  

Sau khi làm việc với hàng đợi cần phải xóa nó bằng lệnh:   
````
int msgctl(int msqid, int cmd, struct msqid_ds *buf);
````
trong đó:  
`int msqid` – định danh hàng đợi;
`int cmd`– lệnh điều khiển (control command) để xóa, IPC_RMID;
`struct msqid_ds *buf` – bộ đệm để thiết lập và nhận thông tin về hàng đợi, bị bỏ qua trong trường hợp của lệnh xóa.  

## Mô tả công việc:  
Viết bộ 2 chương trình, một để truyền tin nhắn vào hàng đợi tin nhắn, và chương trình còn lại  - chấp nhận tin nhắn từ hàng đợi tin nhắn. Kiểm tra công việc của hàm khi bị block và unblock. Có thể lựa chọn chuẩn SVID hoặc chuẩn POSIX.  
````
//Program1
declare the end flag
declare a message queue id;
Stream function ()
{
declare a buffer;
yet (the thread termination flag is not set)
{
create a message in the buffer;
write the message from the buffer to the message queue;
delay for a while;
}
}
main program ()
{
declare a thread id;
create (or open if there is) a message queue;
create a thread from the thread's function;
wait for key presses;
set the end of thread flag;
wait for the thread to complete;
close the message queue;
delete the message queue;
}
````
````
//Program2
declare the end flag
declare a message queue id;
Stream function ()
{
declare a buffer;
yet (the thread termination flag is not set)
{
clear message buffer;
accept a message from the message queue to the buffer;
display a message;
}
}
main program ()
{
declare a thread id;
create (or open if there is) a message queue;
create a thread from the thread function;
wait for key presses;
set the end of thread flag;
wait for the thread to complete;
close the message queue;
delete the message queue;
}
````


