// Author: Dmitry Kukovinets (d1021976@gmail.com)

inline MainWindow * CryptoKernelAgent::mainWindow() const
{ return this->mainWindow_; }

inline bool CryptoKernelAgent::addMainWindow(MainWindow *mainWindow)
{
	if (this->mainWindow_ == nullptr) {
		this->mainWindow_ = mainWindow;
		return true;
	}
	return false;
}

inline bool CryptoKernelAgent::removeMainWindow()
{
	if (this->mainWindow_ != nullptr) {
		this->mainWindow_ = nullptr;
		return true;
	}
	return false;
}


inline QString CryptoKernelAgent::typeName(RecordItem *recordItem) const
{
	try {
		auto recordId = this->recordItemsMap_.at(recordItem).first;
		auto typeId = this->kernel_.record_type(recordId);
		auto typeItem = this->typeIdsMap_.at(typeId);
		return typeItem->name();
	} catch (...) {}
	return QString();
}

inline QString CryptoKernelAgent::parentGroupName(RecordItem *recordItem) const
{
	try {
		auto recordId = this->recordItemsMap_.at(recordItem).first;
		auto parentGroupId = this->kernel_.record_parent_group(recordId);
		auto parentGroupItem = this->groupIdsMap_.at(parentGroupId);
		return parentGroupItem->name();
	} catch (...) {}
	return QString();
}


inline void CryptoKernelAgent::showRecordInList(RecordItem *recordItem)
{
	try {
		this->recordItemsMap_.at(recordItem).second->setHidden(false);
	} catch (...) {}
}

inline void CryptoKernelAgent::hideRecordInList(RecordItem *recordItem)
{
	try {
		this->recordItemsMap_.at(recordItem).second->setHidden(true);
	} catch (...) {}
}


inline bool connect(CryptoKernelAgent *agent, MainWindow *mainWindow)
{
	if (agent->addMainWindow(mainWindow) && mainWindow->addAgent(agent))
		return true;
	return false;
}

inline bool disconnect(CryptoKernelAgent *agent, MainWindow *mainWindow)
{
	if (agent->mainWindow() == mainWindow && mainWindow->agent() == agent) {
		agent->removeMainWindow();
		mainWindow->removeAgent();
		return true;
	}
	return false;
}
