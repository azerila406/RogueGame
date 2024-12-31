#include "game.h"

const char EMAIL_CHAR[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
bool isCorrectFormEmail(const char *s) {
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
  return at_sign == 1 && dot == 1;
}

bool isCorrectFormPass(const char *pass) {
  int n = strlen(pass);
  int num = 0, bigAlpha = 0, smallAlpha = 0;
  for (int i = 0; i < n; ++i) {
    num += '0' <= pass[i] && pass[i] <= '9';
    bigAlpha += 'A' <= pass[i] && pass[i] <= 'Z';
    smallAlpha += 'a' <= pass[i] && pass[i] <= 'z';
  }
  return n >= 7 && num && bigAlpha && smallAlpha;
}

// TODO
bool doesUserExists(const char *user) { return 0; }