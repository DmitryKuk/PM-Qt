// Author: Dmitry Kukovinets (d1021976@gmail.com)

inline QString AbstractItem::name() const
{
	return this->text(0);
}

inline void AbstractItem::setName(const QString &newName)
{
	this->setText(0, newName);
}
