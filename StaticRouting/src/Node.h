#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class Interface;
#include "Interface.h"

void set_console_error();
void reset_console_color();
void set_console_warning();
void set_console_success();
void set_console_info();

class Node
{
public:
    Node(const std::string& label) : m_label(label), m_interface_count(0) {}
    void set_label(std::string& label) { m_label = label; }
    void add_interface(std::string& ip);
    void add_interface(std::string& ip1, std::string& ip2, Node* neighbor);
    void remove_neighborless_interface(std::string& iface, bool formatted = true);
    void remove_interface_w_neighbor(std::string& label, bool silent = true);
    void clear_neighborless_interfaces() { m_neighborless_interfaces.clear(); }
    //void set_interfaces_w_neighbor(std::vector<std::string[2]>& interfaces);
    std::string get_label() { return m_label; }
    size_t get_interface_count() { return m_interface_count;  }
    const auto& get_iinterfaces_w_neighbor() const { return  m_interfaces_w_neighbor; }
    const auto& get_neighborless_iinterfaces() const { return  m_neighborless_interfaces; }

private:
    std::string m_label;
    std::unordered_map<std::string, Interface> m_interfaces_w_neighbor;
    std::unordered_map<std::string, Interface> m_neighborless_interfaces;
    size_t m_interface_count;
};