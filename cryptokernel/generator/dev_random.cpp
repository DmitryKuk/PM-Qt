// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "dev_random.h"

generator::raw_dev_random::raw_dev_random(const std::string &dev_name):
	stream_(dev_name, std::ios::in | std::ios::binary)
{
	if (stream_.good())
		this->set_status(generator::status::success);
	else
		this->set_status(generator::status::not_initialized);
}


void
generator::raw_dev_random::generate(void *data, size_t n) const
{
	std::unique_lock<std::mutex> ul(this->mutex_);	// Guarantee async-safety
	
	if (this->status() == generator::status::not_initialized) return;
	
	if (this->stream_.good()) {
		this->stream_.read(static_cast<char *>(data), n);
		if (this->stream_.gcount() != n || !this->stream_.good())
			this->set_status(generator::status::unknown_error);
	} else
		this->set_status(generator::status::unknown_error);
}


generator::universal_dev_random::universal_dev_random(const std::string &dev_name):
	raw_dev_random(dev_name)
{}
