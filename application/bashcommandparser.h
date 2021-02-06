#ifndef BASHCOMMANDPARSER_H
#define BASHCOMMANDPARSER_H

#include "fuzzybashstream.h"

class BashCommandParser
{
  public:
	BashCommandParser(QString command);
	QString command() const { return m_command; }
	QStringList rawArgs() const { return m_args; }

  private:
	QString m_command;
	QStringList m_args;
};

#endif // BASHCOMMANDPARSER_H
