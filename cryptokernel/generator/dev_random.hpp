// Author: Dmitry Kukovinets (d1021976@gmail.com)

template<class Num>
Num
generator::universal_dev_random::generate() const
{
	Num n(0);
	if (this->status() == status::not_initialized) return n;
	
	this->generate(static_cast<void *>(&n), sizeof(Num));
	return n;
}


template<class Num>
generator::dev_random<Num>::dev_random(const std::string &dev_name):
	raw_dev_random(dev_name)
{}


template<class Num>
Num
generator::dev_random<Num>::generate() const
{
	Num n(0);
	if (this->status() == status::not_initialized) return n;
	
	this->generate(static_cast<void *>(&n), sizeof(Num));
	return n;
}


template<class Num>
inline
const generator::universal_dev_random &
operator>>(const generator::universal_dev_random &g, Num &n)
{
	n = g.generate<Num>();
	return g;
}
