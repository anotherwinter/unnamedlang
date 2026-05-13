#include "diagnostics.h"
#include "errorcode_bases.h"
#include "front/lexer.h"

Diagnostics::Diagnostics(DiagnosticsKey)
{
  _msgs.reserve(_msgLimit);
}

DiagID
Diagnostics::putMsg(ErrorCode code, size_t line, size_t col, DiagSeverity sev)
{
  if (_msgs.size() == _msgLimit)
    return InvalidDiagID;

  if (sev == DiagSeverity::ERROR)
    _hasErrors = true;

  _msgs.push_back({ code, sev, line, col });
  return _lastMsgID++;
}

const DiagMessage*
Diagnostics::getDiag(DiagID id) const
{
  if (id >= _msgLimit || id >= _msgs.size())
    return nullptr;

  return &_msgs[id];
}

const char*
Diagnostics::getErrorMsg(ErrorCode code) const
{
  return "Stub error";

  if (code < PARS_ERR_BASE)
    return Lexer::errorMsg(static_cast<LexerError>(code));

  // return Parser::errorMsg(static_cast<ParserError>(code));
}

std::string
Diagnostics::getDiagStr(DiagID id) const
{
  if (id >= _msgLimit || id >= _msgs.size())
    return {};

  DiagMessage diag = _msgs[id];
  std::string str = (diag.sev == DiagSeverity::ERROR ? "<Error" : "<Warning");
  str +=
    " (" + std::to_string(diag.line) + "; " + std::to_string(diag.col) + "): ";
  str += getErrorMsg(diag.code);
  str += ">";

  return str;
}

void
Diagnostics::clear()
{
  _lastMsgID = 0;
  _supressedMsgs = 0;
  _hasErrors = false;

  _msgs.clear();
}