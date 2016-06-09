/*
 * hsm.h
 *
 *  Created on: 2013-8-8
 *      Author: yuandf
 */

#ifndef HSM_H_
#define HSM_H_

typedef unsigned short QSIG;
// Define the signal of state machine
enum
{
    Q_EMPTY_SIG = 0,//空消息，用于检测状态之间的父子关系
    /*<定义初始化状态迁移(initial transition)的消息，接受到该消息后，自动迁移到该状态的状态初始
     * 子状态
     */
    Q_INIT_SIG = 1,
    Q_ENTRY_SIG,//entry 消息
    Q_EXIT_SIG,//exit 消息
    Q_USER_SIG//应用程序使用的状态的起始编号
};

// Define the signal of state machine
typedef struct tagQEVENT
{
  QSIG sig;
  unsigned char *pEvent1;
  unsigned char *pEvent2;
  // TODO: add fields to the event
} QEVENT;

// define state data type
typedef void  (* QPseudoState)(QEVENT const *pEvent);//定义返回为void的函数指针类型
typedef QPseudoState (* QState)(QEVENT const *pEvent);//定义返回为QPseudoState函数指针的函数指针类型
typedef QPseudoState QSTATE;//定义函数指针类型

#define Q_TRIGGER(state, sig) \
   (QState)(*(state))((QEVENT*)&pkgStdEvt[sig])//向状态state发送 sig信号

// define a transation that don't change the state,
// just treat the pEvent with the target state.
// this is used by concurrent state
#define Q_INIT(target_) Init_((QState)(target_));
#define Q_TRAN(target_) Q_TranDyc((QState)(target_));

void Init_(QState target);
void Q_Init(QSTATE target);
void Q_Initial(QEVENT const* pQevt);
void Q_Dispatch(QEVENT const* pQevt);
void Q_TranDyc(QState target);


#endif /* HSM_H_ */
