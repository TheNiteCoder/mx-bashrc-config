#include "bashcommandparser.h"

BashCommandParser::BashCommandParser(QString command)
{
	FuzzyBashStream stream{command};
}
