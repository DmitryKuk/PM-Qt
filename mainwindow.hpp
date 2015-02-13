// Author: Dmitry Kukovinets (d1021976@gmail.com)

inline bool MainWindow::needSaveSettings() const
{ return this->saveSettings_; }

inline void MainWindow::setSaveSettings(bool enable)
{ this->saveSettings_ = enable; }


inline LeftPanelWidget * MainWindow::leftPanelWidget()
{ return this->leftPanelWidget_; }

inline GroupListWidget * MainWindow::groupListWidget()
{ return this->leftPanelWidget()->groupListWidget() ; }

inline TaskListWidget * MainWindow::taskListWidget()
{ return this->leftPanelWidget()->taskListWidget(); }


inline MainWidget * MainWindow::mainWidget()
{ return this->mainWidget_; }

inline RecordListWidget * MainWindow::recordListWidget()
{ return this->mainWidget()->recordListWidget(); }

inline RecordContentWidget * MainWindow::recordContentWidget()
{ return this->mainWidget()->recordContentWidget(); }

inline CryptoKernelAgent * MainWindow::agent()
{ return this->agent_; }
