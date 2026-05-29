#pragma once
#include <string>

class Customer {
public:
    Customer(std::string  firstName,
             std::string  lastName,
             std::string  phone,
             std::string  email);

    std::string getFullName() const;
    std::string getPhone() const;
    std::string getEmail() const;

    void setPhone(const std::string& newPhone);
    void setEmail(const std::string& newEmail);
    void setFirstName(const std::string& newFirstName);
    void setLastName(const std::string& newLastName);
    void display() const;

private:
    std::string firstName;
    std::string lastName;
    std::string phoneNumber;
    std::string emailAddress;
};