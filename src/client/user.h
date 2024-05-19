#ifndef USER_H
#define USER_H

#include <string>

class User {
private:
    std::string username;

    // TODO: add matches_played and matches_won

public:
    User();
    void setUsername(const std::string& name);
    std::string getUsername() const;
};

#endif // USER_H
