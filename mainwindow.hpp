// Author: Dmitry Kukovinets (d1021976@gmail.com)

inline bool MainWindow::needSaveSettings() const
{ return this->saveSettings_; }

inline void MainWindow::setSaveSettings(bool enable)
{ this->saveSettings_ = enable; }


inline LeftPanelWidget * MainWindow::leftPanelWidget() const
{ return this->leftPanelWidget_; }


inline MainWidget * MainWindow::mainWidget() const
{ return this->mainWidget_; }


inline CryptoKernelAgent * MainWindow::agent() const
{ return this->agent_; }

inline bool MainWindow::addAgent(CryptoKernelAgent *agent)
{
	if (this->agent_ == nullptr) {
		this->agent_ = agent;
		return true;
	}
	return false;
}

inline bool MainWindow::removeAgent()
{
	if (this->agent_ != nullptr) {
		this->agent_ = nullptr;
		return true;
	}
	return false;
}
