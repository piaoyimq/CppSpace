/*
 * SequenceId.cpp
 *
 *  Created on: 2016年6月4日
 *      Author: root
 */

class SequenceId
{
public:
	explicit SequenceId();
	explicit SequenceId(uint32_t value);
	SequenceId(const SequenceId& sequenceId);
	virtual ~SequenceId();

	virtual void serialize(boost::archive::binary_oarchive& archive) const;
	virtual void unserialize(boost::archive::binary_iarchive& archive);

	SequenceId& operator=(const SequenceId& sequenceId);
	bool operator==(const SequenceId& sequenceId) const;
	bool operator <(const SequenceId& other) const;

	std::ostream& print(std::ostream& os) const;

	static SequenceId generateNextId();
	static const SequenceId& null();

private:
	friend class boost::serialization::access;

	template<class Archive>
	inline void serialize(Archive & ar, const unsigned int /* file_version */);

	uint32_t value_;
};


template<class Archive>
void SequenceId::serialize(Archive & ar, const unsigned int /* file_version */)
{
	ar & value_;
}
