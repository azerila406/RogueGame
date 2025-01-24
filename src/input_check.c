#include "game.h"

const char EMAIL_CHAR[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
bool isCorrectFormEmail(const char *s, bool error) {
  int n = strlen(s), m = strlen(EMAIL_CHAR), dot = 0, at_sign = 0;
  for (int i = 0; i < n; i++) {
    bool f = 0;
    for (int j = 0; j < m; j++) f |= s[i] == EMAIL_CHAR[j];
    if (f) continue;
    if (s[i] == '@')
      dot = 0, at_sign++;
    else if (s[i] == '.')
      dot++;
    else
      return 0;
  }
  if (at_sign == 1 && dot == 1) return 1;
  if (error) renderMsgAndWait("Email is not in correct form", 1);
  return 0;
}

bool isCorrectFormPass(const char *pass, bool error) {
  int n = strlen(pass);
  int num = 0, bigAlpha = 0, smallAlpha = 0;
  for (int i = 0; i < n; ++i) {
    num += '0' <= pass[i] && pass[i] <= '9';
    bigAlpha += 'A' <= pass[i] && pass[i] <= 'Z';
    smallAlpha += 'a' <= pass[i] && pass[i] <= 'z';
  }
  if (n >= 7 && num && bigAlpha && smallAlpha) return 1;
  if (error) {
    if (n < 7)
      renderMsgAndWait("Password is less than 7 char", 1);
    else if (!num)
      renderMsgAndWait("Password doesn't have a digit", 1);
    else if (!bigAlpha)
      renderMsgAndWait("Password doesn't have a capital letter", 1);
    else if (!smallAlpha)
      renderMsgAndWait("Password doesn't have a small letter", 1);
    else
      renderMsgAndWait("Something went wrong...", 1);
  }
  return 0;
}

// TODO
bool doesUserExists(const char *user, bool error) { return 0; }