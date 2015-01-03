// Author: Dmitry Kukovinets (d1021976@gmail.com)

// Id (number)
template<class Id>
inline void io::read(std::istream &stream, Id &id)
{
	stream >> id;
	if (stream.get() != ' ' || !stream) throw (io::exception());
}

template<class Id>
inline void io::write(std::ostream &stream, const Id &id)
{
	if (!(stream << id << ' ')) throw (io::exception());
}


// String
template<>
inline void io::read(std::istream &stream, std::string &str)
{
	size_t size;
	io::read(stream, size);
	io::read_raw_string(stream, str, size);
	if (stream.get() != ' ' || !stream) throw (io::exception());
}

template<>
inline void io::write(std::ostream &stream, const std::string &str)
{
	io::write(stream, str.size());
	io::write_raw_string(stream, str);
	if (!(stream << ' ')) throw (io::exception());
}

inline void io::read_raw_string(std::istream &stream, std::string &str, size_t len)
{
	char data[len + 1];
	data[len] = '\0';
	if (!stream.read(data, len))
		throw (io::exception());
	str = data;
}

inline void io::write_raw_string(std::ostream &stream, const std::string &str)
{
	if (!stream.write(str.c_str(), str.size()))
		throw (io::exception());
}


// Double_map
template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
void io::read(std::istream &stream, double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2> &dmap)
{
	dmap.clear();
	
	size_t size = 0;
	Key1 k1;
	Key2 k2;
	Value val;
	
	io::read(stream, size);
	while (size--) {
		io::read(stream, k1);
		io::read(stream, k2);
		io::read(stream, val);
		
		dmap.insert(std::move(make_triple(k1, k2, val)));
	}
}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
void io::write(std::ostream &stream, const double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2> &dmap)
{
	io::write(stream, dmap.size());
	for (const auto &x: dmap) {
		io::write(stream, x.first);
		io::write(stream, x.second);
		io::write(stream, x.third);
	}
}
