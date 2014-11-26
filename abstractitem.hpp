// Author: Dmitry Kukovinets (d1021976@gmail.com)

inline id_t AbstractItem::id() const
{
	return this->id_;
}

inline const QString & AbstractItem::name() const
{
	return this->text(0);
}

inline void AbstractItem::setName(const QString &newName)
{
	this->setText(0, newName);
}
