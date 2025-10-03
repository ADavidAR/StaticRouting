#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <cctype>
#include <unordered_set>
#include <fstream>

#include "Graph.h"

class Validator {
public:
    static std::vector<std::string> route_from_node(Node* node)
    {

        std::vector<std::string> routes;

        for (const auto& [l, i] : node->get_iinterfaces_w_neighbor())
        {

            std::unordered_set<std::string> r;
            std::unordered_set<std::string> v {node->get_label()};
            m_graph_traversal(i.get_neighbor(), r, v);

            for (const auto& net : r)
            {
                routes.push_back(net + " " + Interface::bin_to_dotted(i.get_neighbor_ip_address()));
            }
        }
        return routes;
    }

    static void save_to_file(const std::string& filename, std::vector<std::string> lines, std::string prefix = "")
    {
        std::ofstream f(filename);
        
        for (const auto& l : lines)
            f << prefix << l << "\n";
        f.close();
    }

    static std::vector<std::string> sep_ips(std::string str)
    {
        std::istringstream iss(str);

        std::vector<std::string> ips;
        for (std::string elem; std::getline(iss, elem, ',');)
            ips.push_back(trim(elem));

        return ips;
    }
    static std::string trim(const std::string& str)
    {
        size_t start = str.find_first_not_of(" \t\n\r");
        size_t end = str.find_last_not_of(" \t\n\r");

        if (start == std::string::npos) return "";

        return str.substr(start, end - start + 1);
    }

    static bool validate_ip(const std::string& input)
    {
        std::string trimmed = trim(input);
        if (trimmed.empty()) return false;

        if (trimmed.find('/') != std::string::npos)
        {
            return m_validate_ip_w_CIDR(trimmed);
        }

        return m_validate_ip_w_mask(trimmed);

    }


private:
    static void m_graph_traversal(Node* u, std::unordered_set<std::string>&routes, std::unordered_set<std::string>& visited)
    {
        if (visited.find(u->get_label()) == visited.end())
        {
            visited.insert(u->get_label());
            for (const auto& [l, i] : u->get_neighborless_iinterfaces())
                routes.insert(i.get_full_dotted_network_ip_address());
        }

        for (const auto&[l, i] : u->get_iinterfaces_w_neighbor())
        {
            Node* v = i.get_neighbor();
            if (visited.find(v->get_label()) == visited.end())
                m_graph_traversal(v, routes, visited);
        }
    }

    static std::string m_join_vec(std::vector<std::string> vec, size_t start, size_t end)
    {
        std::string joined = (start < vec.size()) ? vec[start] : "";
        end = (end <= vec.size()) ? end : vec.size();
        ++start;
        for (; start < end; ++start) { joined += " " + vec[start]; }

        return joined;
    }

    static std::string m_join_vec(std::vector<std::string> vec, size_t start)
    {
        return m_join_vec(vec, start, vec.size());
    }

    static bool m_is_number(const std::string& str)
    {
        if (str.empty()) return false;
        
        for (char c : str) 
            if (!std::isdigit(c)) return false;
        
        return true;
    }

    static bool m_is_valid_octet(const std::string& octetStr)
    {
        if (!m_is_number(octetStr)) return false;
        
        int octet = std::stoi(octetStr);
        
        return (octet >= 0 && octet <= 255);
    }

    static bool m_is_valid_CIDR(int cidr) { return (cidr >= 0 && cidr <= 32); }

    static bool m_is_valid_subnet_mask(const std::vector<int>& octets) 
    {
        if (octets.size() != 4) return false;
        
        unsigned int mask = (octets[0] << 24) | (octets[1] << 16) | (octets[2] << 8) | octets[3];
        
        if (mask == 0) return false;
        
        unsigned int complement = ~mask;
        return (complement & (complement + 1)) == 0;
    }

    static std::vector<int> m_parse_ip_address(const std::string& ipStr)
    {
        std::vector<int> result;
        std::istringstream iss(ipStr);
        std::string octet;

        while (std::getline(iss, octet, '.')) 
        {
            result.push_back(std::stoi(octet));
        }
        return result;
    }

    static bool m_validate_ip_address(const std::string& ipStr)
    {
        std::string trimmed = trim(ipStr);
        std::vector<std::string> octets;
        std::istringstream iss(trimmed);
        std::string octet;

        while (std::getline(iss, octet, '.')) 
            octets.push_back(octet);

        if (octets.size() != 4) return false;

        for (const auto& octetStr : octets)
            if (!m_is_valid_octet(octetStr)) return false;

        return true;
    }

    static bool m_validate_ip_w_mask(const std::string& input)
    {
        std::string trimmed = trim(input);
        std::vector<std::string> parts;
        std::istringstream iss(trimmed);
        std::string part;

        while (iss >> part) {
            parts.push_back(part);
        }

        if (parts.size() != 2) return false;

        if (!m_validate_ip_address(parts[0])) return false;

        if (parts[1].find('.') != std::string::npos) {
            return m_validate_ip_address(parts[1]) && m_is_valid_subnet_mask(m_parse_ip_address(parts[1]));
        }
        else {
            if (!m_is_number(parts[1])) return false;
            int cidr = std::stoi(parts[1]);
            return m_is_valid_CIDR(cidr);
        }
    }

    static bool m_validate_ip_w_CIDR(const std::string& input)
    {
        std::string trimmed = trim(input);
        size_t slashPos = trimmed.find('/');
        if (slashPos == std::string::npos) return false;

        std::string ipPart = trim(trimmed.substr(0, slashPos));
        std::string cidrPart = trim(trimmed.substr(slashPos + 1));

        if (!m_validate_ip_address(ipPart)) return false;
        if (!m_is_number(cidrPart)) return false;

        int cidr = std::stoi(cidrPart);
        return m_is_valid_CIDR(cidr);
    }
};