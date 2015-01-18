// Author: Dmitry Kukovinets (d1021976@gmail.com)

inline bool MainWindow::needSaveSettings() const
{ return this->saveSettings_; }

inline void MainWindow::setSaveSettings(bool enable)
{ this->saveSettings_ = enable; }


inline LeftPanelWidget * MainWindow::leftPanelWidget() const
{ return this->leftPanelWidget_; }

inline GroupListWidget * MainWindow::groupListWidget() const
{ return this->leftPanelWidget()->groupListWidget() ; }

inline TaskListWidget * MainWindow::taskListWidget() const
{ return this->leftPanelWidget()->taskListWidget(); }


inline MainWidget * MainWindow::mainWidget() const
{ return this->mainWidget_; }

inline RecordListWidget * MainWindow::recordListWidget() const
{ return this->mainWidget()->recordListWidget(); }

inline RecordContentWidget * MainWindow::recordContentWidget() const
{ return this->mainWidget()->recordContentWidget(); }

inline CryptoKernelAgent * MainWindow::agent() const
{ return this->agent_; }
