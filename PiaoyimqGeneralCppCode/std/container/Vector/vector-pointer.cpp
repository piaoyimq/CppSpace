#include <iostream>
#include <vector>
#include <memory>


void test_vector_element_is_normal_pointer()
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


void test_vector_has_unique_ptr_element()
{
	typedef std::vector<std::unique_ptr<DbpRoute> > DbpRouteList;
	DbpRouteList routesList;
	auto it = routesList.begin();
	auto route = std::unique_ptr < DbpRoute > (new DbpRoute());// <==> std::unique_ptr<DbpRoute> route(new DbpRoute());
	routesList.insert(it, std::move(route));// have to transfer to rvalue.
	routesList.push_back(std::move(route));

	DbpRouteList routesListCopy(routesList); //could not like this, unique_ptr not support copy constructor,
		//but can like this:  DbpRouteList routesListCopy(std::move(routesList)); , but we should implment the move constructor.

	for (const auto& listRoute : routesList)
	{
		it = routesList.erase(it);
	}
}


class Song
{
public:
	Song(string a, string t) :
			artist(a), title(t)
	{
	}

	void print(const char* objectName)
	{
		cout << objectName << ": " << artist << ", " << title << endl;
	}
private:
	string artist;
	string title;
};


void SongVector()
{
	vector < unique_ptr < Song >> songs;

	// Create a few new unique_ptr<Song> instances
	// and add them to vector using implicit move semantics.
	songs.push_back(make_unique < Song > (L"B'z", L"Juice"));// make_unique return a rvalue, so no need to use std::move().
	songs.push_back(make_unique < Song > (L"Namie Amuro", L"Funky Town"));
	songs.push_back(make_unique < Song > (L"Kome Kome Club", L"Kimi ga Iru Dake de"));
	songs.push_back(make_unique < Song > (L"Ayumi Hamasaki", L"Poker Face"));

	// Pass by const reference when possible to avoid copying.
	for (const auto& song : songs)
	{
		cout << "Artist: " << song->artist << "   Title: " << song->title << endl;
	}
}


void test_vector_pointer()
{
	std::vector<InAddr2_t>*     pPcscfAddrV4;
	pPcscfAddrV4 = new std::vector<InAddr2_t>;
	pPcscfAddrV4->push_back(InAddr2_t("121.51.61.71"));
}
void test_vector_pointer2()
{
	std::unique_ptr<std::vector<PcscfAddrNode> > pcscfAddrNodeVec(new std::vector<PcscfAddrNode>());

}


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


void test_vector_has_shared_ptr()
{
	typedef std::vector<std::shared_ptr<IPeerRuntime> > PeerRuntimeList;
	PeerRuntimeList                            peerRuntimes_;
	auto peerRuntime = std::make_shared<PeerRuntime>();
	peerRuntimes_.push_back(peerRuntime);//no need to use std::move, because of it was shared_ptr.
	PeerRuntimeList peerRuntimes_Copy(peerRuntimes_);//it will call copy constructor.
}
