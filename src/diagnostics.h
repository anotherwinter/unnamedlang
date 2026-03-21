#pragma once
#include "errorcode_bases.h"
#include <cstddef>
#include <limits>
#include <string>
#include <vector>

enum class DiagSeverity
{
  ERROR,
  WARNING
};

struct DiagMessage
{
  ErrorCode code;
  DiagSeverity sev;
  size_t line;
  size_t col;
};

using DiagID = uint32_t;
static_assert(sizeof(DiagID) == 4, "DiagID must be 4 bytes");

constexpr DiagID InvalidDiagID = std::numeric_limits<DiagID>::max();
constexpr size_t MessagesLimit = 100;

class Diagnostics
{
public:
  struct DiagnosticsKey {
    private:
      friend class Initializer;
      DiagnosticsKey() = default;
  };

  Diagnostics(DiagnosticsKey);
  inline size_t getSupressedCount() const { return _supressedMsgs; }
  inline size_t getMsgCount() const { return _msgs.size(); }
  inline bool hasErrors() const { return _hasErrors; }
  DiagID putMsg(ErrorCode code, DiagSeverity sev, size_t line, size_t col);
  const DiagMessage* getDiag(DiagID id) const;
  const char* getErrorMsg(ErrorCode code) const;
  std::string getDiagStr(DiagID id) const;

  // clears internal state and any messages
  void clear();

private:
  Diagnostics(const Diagnostics& other) = delete;
  Diagnostics(Diagnostics&& other) = delete;
  DiagID _lastMsgID;
  size_t _msgLimit;
  std::vector<DiagMessage> _msgs;
  size_t _supressedMsgs;
  bool _hasErrors;
};