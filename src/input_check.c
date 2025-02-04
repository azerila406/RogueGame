#include "game.h"

const char EMAIL_CHAR[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
bool isCorrectFormEmail(const char *s, bool error) {
  int n = strlen(s), m = strlen(EMAIL_CHAR), dot = 0, at_sign = 0, dot_pos = -1, at_pos = -1;
  for (int i = 0; i < n; i++) {
    bool f = 0;
    for (int j = 0; j < m; j++) f |= s[i] == EMAIL_CHAR[j];
    if (f) continue;
    if (s[i] == '@')
      dot = 0, at_sign++, at_pos = i;
    else if (s[i] == '.')
      dot++, dot_pos = i;
    else
      return 0;
  }
  if (at_sign == 1 && dot == 1 && at_pos > 0 && dot_pos > at_pos + 1 && dot_pos != n - 1) return 1;
  if (error) renderMsgAndWait("Email is not in correct form", 3);
  return 0;
}

bool isCorrectFormPass(const char *pass, bool error) {
  int n = strlen(pass);
  int num = 0, bigAlpha = 0, smallAlpha = 0;
  for (int i = 0; i < n; ++i) {
    num += '0' <= pass[i] && pass[i] <= '9';
    bigAlpha += 'A' <= pass[i] && pass[i] <= 'Z';
    smallAlpha += 'a' <= pass[i] && pass[i] <= 'z';
    if (pass[i] == '\'') {
      if (error) renderMsgAndWait("Password can't contain this <'>", 3);
      return 0;
    }
  }
  if (n >= 7 && num && bigAlpha && smallAlpha) return 1;
  if (error) {
    if (n < 7)
      renderMsgAndWait("Password is less than 7 char", 3);
    else if (!num)
      renderMsgAndWait("Password doesn't have a digit",3);
    else if (!bigAlpha)
      renderMsgAndWait("Password doesn't have a capital letter", 3);
    else if (!smallAlpha)
      renderMsgAndWait("Password doesn't have a small letter", 3);
    else
      renderMsgAndWait("Something went wrong...", 3);
  }
  return 0;
}