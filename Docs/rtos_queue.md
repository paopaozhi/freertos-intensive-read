# 通过队列传递指针

> 发送指针时发送指针

```c
//传递结构体的指针
/* 创建10个存储指针变量的消息队列，由于CM3/CM4内核是32位机，一个指针变量占用4个字节 */
xQueue = xQueueCreate(10, sizeof(struct Msg *));    //单元大小为一个指针变量的大小

//发送消息，实现结构体指针的传递 
MSG_T *ptMsg;        //MSG_T为结构体声明
ptMsg = &g_tMsg;   /* 初始化结构体指针 */
// 初始化数组 
ptMsg->ucMessageID = 0;
ptMsg->ulData[0] = 0;
ptMsg->usData[0] = 0;
//使用消息队列实现指针变量的传递 
if (xQueueSend(xQueue2,                  /* 消息队列句柄 */
               (void *) &ptMsg,           // 发送结构体指针变量ptMsg的地址  “&”  取结构体指针的地址，传递指针
               (TickType_t) 10) != pdPASS)

//接收消息，接收结构体的指针
    MSG_T *ptMsg; //定义一个结构体指针
xResult = xQueueReceive(xQueue2,            /* 消息队列句柄 */
                        (void *) &ptMsg,      // 这里获取的是结构体的地址,类似于 char *a="stm";char *b;b=a 指针赋值，a和b指向同一个地址
                        (TickType_t) xMaxBlockTime);/* 设置阻塞时间 */
if (xResult == pdPASS) {
    /* 成功接收，并通过串口将数据打印出来 */
    printf("接收到消息队列数据ptMsg->ucMessageID = %d\r\n", ptMsg->ucMessageID);
    printf("接收到消息队列数据ptMsg->ulData[0] = %d\r\n", ptMsg->ulData[0]);
    printf("接收到消息队列数据ptMsg->usData[0] = %d\r\n", ptMsg->usData[0]);
} 

```