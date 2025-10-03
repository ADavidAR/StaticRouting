#include "Interface.h"



Interface::Interface(std::string& str_ip1, std::string& str_ip2, Node* n)
{
    Ip ip1 = m_string_to_ip(str_ip1);
    Ip ip2 = m_string_to_ip(str_ip2);


    if (ip1.ip_address == ip2.ip_address)
        throw IpException("Direcciones IP iguales. Las direcciones deben ser distintas");
    
    if (ip1.mask_length != ip2.mask_length)
        throw IpException("Direcciones IP con distinta mascara. Las direcciones deben pertenecer a la misma red");

    m_mask_length = ip1.mask_length;

    int mask = 0xffffffff << (32 - m_mask_length);

    if ((ip1.ip_address & mask) != (ip2.ip_address & mask))
        throw IpException("Direcciones IP de distintos segementos de red. Las direcciones deben pertenecer a la misma red");

    m_ip_address1 = ip1.ip_address;
    m_ip_address2 = ip2.ip_address;

    m_network_ip_address = m_ip_address1 & mask;

    m_neighbor_node = n;
}

Interface::Interface(std::string& str_ip) 
    : m_ip_address2(0), m_neighbor_node(nullptr)
{
    Ip ip = m_string_to_ip(str_ip);

    m_mask_length = ip.mask_length;

    int mask = 0xffffffff << (32 - m_mask_length);
    m_ip_address1 = ip.ip_address;

    m_network_ip_address = m_ip_address1 & mask;
}


std::string Interface::get_dotted_mask() const
{
    int mask = 0xffffffff << (32 - m_mask_length);
    return bin_to_dotted(mask);
}

std::string Interface::get_dotted_network_ip_address() const
{
    return bin_to_dotted(m_network_ip_address);
}

std::string Interface::get_full_dotted_network_ip_address () const
{
    return get_dotted_network_ip_address() + " " + get_dotted_mask();
}

int Interface::dotted_to_bin(std::string& dotted)
{
    std::istringstream iss(dotted);
    int bits = 0;
    for (std::string elem; std::getline(iss, elem, '.');)
    {
        bits = (bits << 8) | std::stoi(elem);
    }

    return bits;
}

std::string Interface::bin_to_dotted(int bits)
{
    std::string dotted = std::to_string((bits >> 24) & 0xff) + "."
        + std::to_string((bits >> 16) & 0xff) + "."
        + std::to_string((bits >> 8) & 0xff) + "."
        + std::to_string(bits & 0xff);

    return dotted;
}

std::string Interface::format_ip_address(std::string& str_ip)
{
    Ip ip = m_string_to_ip(str_ip);
    std::string format = bin_to_dotted(ip.ip_address & (0xffffffff << (32 - ip.mask_length))) + "/" + std::to_string(ip.mask_length);                        
    return format;
}

Ip Interface::m_string_to_ip(std::string& ip)
{
    Ip res;
    
    if (ip.find("/") != ip.npos)
    {
        std::istringstream iss(ip);
        std::vector<std::string> ip_elems;
        std::erase_if(ip, [](unsigned char c) { return !isdigit(c) && c != '.' && c != '/'; });
        for (std::string elem; std::getline(iss, elem, '/'); )
            ip_elems.push_back(elem);


        res.ip_address = dotted_to_bin(ip_elems[0]);
        res.mask_length = (ip_elems.size() < 2) ? 24 : static_cast<short>(std::stoi(ip_elems[1]));

        return res;
    }
    std::istringstream iss(ip);

    std::vector<std::string> ip_elems((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

    res.ip_address = dotted_to_bin(ip_elems[0]);
    res.mask_length = (ip_elems.size() < 2) ? 24 : static_cast<short>(std::popcount(static_cast<unsigned int>(dotted_to_bin(ip_elems[1]))));

    return res;
}