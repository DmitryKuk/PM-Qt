// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "dev_random.h"

// Static
std::ifstream generator::raw_dev_random::stream_(DEV_RANDOM_DEVICE, std::ios::in | std::ios::binary);
std::mutex generator::raw_dev_random::mutex_;


// Default constructor
generator::raw_dev_random::raw_dev_random():	// Default
	generator::base()
{
	generator::raw_dev_random::mutex_.lock();	// Guarantee async-safety
	bool b = generator::raw_dev_random::stream_.good();
	generator::raw_dev_random::mutex_.unlock();
	
	if (b)	this->set_status(generator::status::success);
	else	this->set_status(generator::status::not_initialized);
}


void
generator::raw_dev_random::operator()(void *data, size_t n) const
{
	generator::raw_dev_random::mutex_.lock();	// Guarantee async-safety
	if (this->status() == generator::status::not_initialized) return;
	
	if (this->stream_.good()) {
		this->stream_.read(static_cast<char *>(data), n);
		bool b = !this->stream_.good() || this->stream_.gcount() != n;
		generator::raw_dev_random::mutex_.unlock();
		if (b) this->set_status(generator::status::unknown_error);
	} else {
		generator::raw_dev_random::mutex_.unlock();
		this->set_status(generator::status::unknown_error);
	}
}
