#ifndef FUZZYBASHSTREAM_H
#define FUZZYBASHSTREAM_H

#include <QString>
#include <QList>
#include <QDebug>

class FuzzyBashStream
{
public:
	enum TokenType
	{
		TokenUnknown = -1,
		TokenEOL = 0,
		TokenQuoted,
		TokenWord,
		TokenWhitespace,
		TokenOpeningSquareBracket,
		TokenClosingSquareBracket,
		TokenOpeningCurlyBracket,
		TokenClosingCurlyBracket,
		TokenOpeningParentheses,
		TokenClosingParentheses,
		TokenEqualSign,
		TokenNotEqualSign,
		TokenRegexMatchSign,
		TokenOr,
		TokenPipe,
		TokenAnd,
		TokenFork,
		TokenComment,

		// TOKENIZEv2 token types

		TokenCommand,
		TokenBooleanExpression,
    };
private:
    struct Token
    {
        int tokenType = TokenUnknown;
        int start = -1, end = -1;
        QString content;
        bool isValid() const { return start >= 0 && end >= 0; }
    };
    using TokenList = QList<Token>;
public:

    class TokenRef
    {
    public:
        void setContent(const QString& content) { m_token.content = content; commit(); }
        QString content() const { return m_token.content; }
        TokenType type() const { return static_cast<TokenType>(m_token.tokenType); }
    private:
        TokenRef(const Token& t, FuzzyBashStream* stream);
        void commit();
        Token m_token;
        FuzzyBashStream* m_stream;
        friend class FuzzyBashStream;
    };

    FuzzyBashStream(QString source);
    QString source() const { return m_source; }
    void reparse();
    QList<TokenRef> tokens() const { return m_tokenRefs; }


private:
    QList<TokenRef> m_tokenRefs;
    QString m_source;
    int m_offset;

    void commitTokenRef(const TokenRef& tr);

	enum ParsingMode
	{
		Normal = 0,
		InsideSquareBrackets = (1 << 1),
		JustParsedWord = (1 << 2),
	};
	class InputStream
	{
		public:
			InputStream(QString input) : m_input(input), m_pos(0) {}
			bool eof() const { return !(m_pos < m_input.size()); }
			int pos() const { return m_pos; }
			QChar next() { return m_input[m_pos++]; }
			QChar peek(int offset = 0) const { return m_input[m_pos + offset]; }
			QChar prev(int offset = 0) const { return m_input[m_pos - (offset + 1)]; }
		private:
			const QString m_input;
			int m_pos;
	};
	Token joinTokens(TokenList tokens, int type);
	Token parseToken(InputStream& stream, int& mode);
	TokenList tokenize(InputStream& stream);
	TokenList tokenize2(TokenList tokens);
};

QDebug operator<< (QDebug debug, const FuzzyBashStream::TokenRef& tr);


#endif // FUZZYBASHSTREAM_H
