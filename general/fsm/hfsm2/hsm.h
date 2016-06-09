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
    Q_EMPTY_SIG = 0,//����Ϣ�����ڼ��״̬֮��ĸ��ӹ�ϵ
    /*<�����ʼ��״̬Ǩ��(initial transition)����Ϣ�����ܵ�����Ϣ���Զ�Ǩ�Ƶ���״̬��״̬��ʼ
     * ��״̬
     */
    Q_INIT_SIG = 1,
    Q_ENTRY_SIG,//entry ��Ϣ
    Q_EXIT_SIG,//exit ��Ϣ
    Q_USER_SIG//Ӧ�ó���ʹ�õ�״̬����ʼ���
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
typedef void  (* QPseudoState)(QEVENT const *pEvent);//���巵��Ϊvoid�ĺ���ָ������
typedef QPseudoState (* QState)(QEVENT const *pEvent);//���巵��ΪQPseudoState����ָ��ĺ���ָ������
typedef QPseudoState QSTATE;//���庯��ָ������

#define Q_TRIGGER(state, sig) \
   (QState)(*(state))((QEVENT*)&pkgStdEvt[sig])//��״̬state���� sig�ź�

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
