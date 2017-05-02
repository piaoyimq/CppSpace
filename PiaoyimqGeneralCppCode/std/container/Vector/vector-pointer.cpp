//
//template <typename IpcMsgT> inline void receiveAll(std::vector<IpcMsgT*>& messages, int timeout = -1) throw(CmpTestFramework::ICmpTestException)
//{
//  IpcMsgT* const ipcMsg = static_cast<IpcMsgT*>(receivedMsg->releaseMessage());
// messages.push_back(ipcMsg);
//}
//
//std::vector<IcrPortIf::DeleteReceiverMsg*> recvAll;
//
//receiveAll<IcrPortIf::DeleteReceiverMsg>(recvAll, 0);



void test_vector_element_is_pointer()
{
	  std::vector<const UpdateEvent*>                                receivedUpdateEvents;
      std::unique_ptr<const UpdateEvent> updateEvent(limIfGu31_0->receive<UpdateEvent>());// so std::unique_ptr<const UpdateEvent> simillar to const Update*
	  receivedUpdateEvents.push_back(updateEvent.release());


       std::vector<const UpdateEvent*>::iterator it;

        for (it = receivedUpdateEvents.begin(); it != receivedUpdateEvents.end(); it++)
        {
            std::unique_ptr<const UpdateEvent> updateEvent(*it);//Note: is *it not it.  *it==UpdateEvent pointer, also is receivedUpdateEvents's element.
            *it = nullptr;

            u32 i = 0;
            while (i < updateEvent->getIndexUpdates().size())
            {
                const IndexUpdate& indexUpdate(updateEvent->getIndexUpdates()[i]);

                injectConfirmationInd(*limIfGu31_0, *updateEvent, indexUpdate, peIdGu32_0, ConfirmationType::Active());
                injectConfirmationInd(*limIfGu31_0, *updateEvent, indexUpdate, peIdGu31_0, ConfirmationType::Standby());

                i++;
            }
        }
        receivedUpdateEvents.clear();
}

typedef std::vector<std::unique_ptr<Gx_ChargingRuleDefinition_t>> Dapp_ChargingRuleDefinitionList;
std::vector<std::unique_ptr<Oam_AccessControlRuleReference_t> >  accessControlRuleList;
typedef std::vector<std::unique_ptr<DbpRoute> > DbpRouteList;

std::vector<InAddr2_t>* DataAddr_GetPcscfAddressesWithType(const Data::Session* pSession, Data::PcscfType type);


int test_vector_push_back_with_rvalue()
{
    std::string str = "Hello";
    std::vector<std::string> v;

    // uses the push_back(const T&) overload, which means
    // we'll incur the cost of copying str
    v.push_back(str);
    std::cout << "After copy, str is \"" << str << "\"\n";
    str="good";
    // uses the rvalue reference push_back(T&&) overload,
    // which means no strings will be copied; instead, the contents
    // of str will be moved into the vector.  This is less
    // expensive, but also means str might now be empty.
    v.push_back(std::move(str));
    std::cout << "After move, str is \"" << str << "\"\n";
    std::cout << "The contents of the vector are \"" << v[0]
                                         << "\", \"" << v[1] << "\"\n";
}
