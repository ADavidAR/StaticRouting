#include <filesystem>

#include "Graph.h"
#include "ConsoleColors.h"

#include "Validator.h"

int main() 
{
	namespace fs = std::filesystem;
	bool exit = false;
	std::unordered_map<std::string, int> commands {////
		{"-e", 0},
		{"-an", 1},
		{"-ae", 2},
		{"-dn", 3},
		{"-de", 4},
		{"-rn", 5},
		{"-ai", 6},
		{"-si", 7},
		{"-r", 8},
		{"-ra", 9}
	};
	std::string load_file;

	std::cout << "¿Desea cargar desde un archivo? [s/n]: ";
	std::getline(std::cin, load_file);

	for (auto& c : load_file) 
		c = std::tolower(static_cast<unsigned char>(c));


	Graph g;

	if (load_file == "s")
	{
		std::string filename;
		std::cout << "ingrese la direccion del archivo: ";
		std::getline(std::cin, filename);

		filename += "  "; // deserialize
	}

	std::cout << "\n\n";

	do
	{

		std::cout << "-> ";
		std::string rq;
		std::getline(std::cin, rq);


		if (rq[0] == '-')
		{
			std::istringstream iss_rq(rq);
			std::vector<std::string> rq_vec((std::istream_iterator<std::string>(iss_rq)), std::istream_iterator<std::string>());
			if (commands.count(rq_vec[0]))
			{
				switch (commands[rq_vec[0]])
				{
				case 0:
					exit = true;
					break;
				case 1: //add node
					if (rq_vec.size() > 2)
					{
						set_console_error();
						std::cout << "[ERROR]: La etiqueta de un nodo no deben contener espacios.\n\n";
						reset_console_color();
						continue;
					}

					g.add_node(rq_vec[1]);
					break;
				case 2: //add edge
					{
						if (rq_vec.size() > 1)
						{
							set_console_error();
							std::cout << "[ERROR]: Demasiados parametros para comando \"-ae\". Parametros necesarios: 0\n\n";
							reset_console_color();
							continue;
						}
						std::string params[4];
						int cur = 0;
						for (int i = 0; i < 2; i++)
						{
							bool repeat = true;
							do
							{
								std::cout << "Etiqueta " << i + 1 << " > ";
								std::getline(std::cin, params[i + cur]);
								if (params[i + cur].find(' ') != params[i + cur].npos ||
									params[i + cur].find('\t') != params[i + cur].npos)
								{
									set_console_error();
									std::cout << "[ERROR]: La etiqueta de un nodo no deben contener espacios.\n\n"; 
									reset_console_color();
									continue;
								}
								repeat = false;
							} while (repeat);
						
							repeat = true;
							cur++;
							do
							{
								std::cout << "Direccion IP " << i + 1 << " > ";
								std::getline(std::cin, params[i + cur]);
								if (!Validator::validate_ip(params[i + cur]))
								{
									set_console_error();
									std::cout << "[ERROR]: Direccion IP Invalida\n\n";
									reset_console_color();
									continue;
								}
								repeat = false;
							} while (repeat);

						}
						g.add_edge(params[0], params[1], params[2], params[3]);
					}
					break;
				case 3: //delete node
					if (rq_vec.size() > 2)
					{
						set_console_error();
						std::cout << "[ERROR]: La etiqueta de un nodo no deben contener espacios.\n\n";
						reset_console_color();
						continue;
					}

					g.remove_node(rq_vec[1]);
					break;
				case 4: //delete edge
					if (rq_vec.size() < 3)
					{
						set_console_error();
						std::cout << "[ERROR]: Falta una etiqueta.\n";
						set_console_success();
						std::cout << "          Uso: -de <etiqueta1> <etiqueta2>\n\n";
						reset_console_color();
						continue;
					}
					if (rq_vec.size() > 3)
					{
						set_console_error();
						std::cout << "[ERROR]: Demasiados parametros.\n";
						set_console_success();
						std::cout << "          Uso: -de <etiqueta1> <etiqueta2>\n\n";
						reset_console_color();
						continue;
					}

					g.remove_edge(rq_vec[1], rq_vec[2]);
					break;
				case 5: //rename node
					if (rq_vec.size() < 3)
					{
						set_console_error();
						std::cout << "[ERROR]: Falta una etiqueta.\n";
						set_console_success();
						std::cout << "          Uso: -de <etiqueta_anterior> <etiqueta_nueva>\n\n";
						reset_console_color();
						continue;
					}
					if (rq_vec.size() > 3)
					{
						set_console_error();
						std::cout << "[ERROR]: Demasiados parametros.\n";
						set_console_success();
						std::cout << "          Uso: -de <etiqueta_anterior> <etiqueta_nueva>\n\n";
						reset_console_color();
						continue;
					}

					g.rename_node(rq_vec[1], rq_vec[2]);
					break;
				case 6: //add neighborless interface
					{
						std::string label;
						std::string ips;

						bool repeat = true;
						do
						{
							std::cout << "Etiqueta > ";
							std::getline(std::cin, label);
							if (label.find(' ') != label.npos || label.find('\t') != label.npos)
							{
								set_console_error();
								std::cout << "[ERROR]: La etiqueta de un nodo no deben contener espacios.\n\n";
								reset_console_color();
								continue;
							}
							repeat = false;
						} while (repeat);

						repeat = true;
						do
						{
							std::cout << "Direccion(es) IP (si son varias separar con comas \",\") > ";
							std::getline(std::cin, ips);
							ips = Validator::trim(ips);
							if (ips.empty())
							{
								set_console_error();
								std::cout << "[ERROR]: ingrese al menos una (1) direccion IP.\n\n";
								reset_console_color();
								continue;
							}
							repeat = false;
						} while (repeat);

						Node* n = g.get_node(label);

						for (auto& ip : Validator::sep_ips(ips))
						{
							if (Validator::validate_ip(ip))
							{
								n->add_interface(ip);
								continue;
							}
							set_console_error();
							std::cout << "[ERROR]: Direccion IP Invalida: " << ip << "\n\n";
							reset_console_color();
						}
					}
					break;
				case 7: //set neighborless interface
					{
						std::string label;
						std::string ips;

						bool repeat = true;
						do
						{
							std::cout << "Etiqueta > ";
							std::getline(std::cin, label);
							if (label.find(' ') != label.npos || label.find('\t') != label.npos)
							{
								set_console_error();
								std::cout << "[ERROR]: La etiqueta de un nodo no deben contener espacios.\n\n";
								reset_console_color();
								continue;
							}
							repeat = false;
						} while (repeat);

						repeat = true;
						do
						{
							std::cout << "Direccion(es) IP (si son varias separar con comas \",\") > ";
							std::getline(std::cin, ips);
							ips = Validator::trim(ips);
							if (ips.empty())
							{
								set_console_error();
								std::cout << "[ERROR]: ingrese al menos una (1) direccion IP.\n\n";
								reset_console_color();
								continue;
							}
							repeat = false;
						} while (repeat);

						if (g.node_exists(label))
						{
							Node* n = g.get_node(label);
							n->clear_neighborless_interfaces();

							for (auto& ip : Validator::sep_ips(ips))
							{
								if (Validator::validate_ip(ip))
								{
									n->add_interface(ip);
									continue;
								}
								set_console_error();
								std::cout << "[ERROR]: Direccion IP Invalida: " << ip << "\n\n";
								reset_console_color();
							}
							continue;
						}
						set_console_warning();
						std::cout << "[INFO]: Nodo Inexistente: " << label << "\n\n";
						reset_console_color();
						
					}
					break;
				case 8: // route from spacific node
					if (rq_vec.size() < 2)
					{
						set_console_error();
						std::cout << "[ERROR]: Falta una etiqueta.\n";
						set_console_success();
						std::cout << "          Uso: -r <etiqueta>\n\n";
						reset_console_color();
						continue;
					}
					if (rq_vec.size() > 2)
					{
						set_console_error();
						std::cout << "[ERROR]: Demasiados parametros.\n";
						set_console_success();
						std::cout << "          Uso: -r <etiqueta>\n\n";
						reset_console_color();
						continue;
					}

					if (g.node_exists(rq_vec[1]))
					{
						bool repeat = true;
						std::string dir;
						do
						{
							std::cout << "Directorio > ";
							std::getline(std::cin, dir);

							dir = Validator::trim(dir);
							if (!fs::exists(dir))
							{
								set_console_error();
								std::cout << "[ERROR]: Directorio inexistente: \"\"" << dir << "\"\n\n";
								reset_console_color();
								continue;
							}
							repeat = false;
						} while (repeat);

						dir += "\\" + rq_vec[1] + ".cfg";
						auto routes = Validator::route_from_node(g.get_node(rq_vec[1]));
						Validator::save_to_file(dir, routes, "ip route ");
						set_console_success();
						std::cout << "\n[INFO]: El archivo se escribio correctamente: \"" << dir << "\"\n";
						reset_console_color();
						continue;
					}
					set_console_warning();
					std::cout << "[INFO]: Nodo Inexistente: " << rq_vec[1] << "\n\n";
					reset_console_color();

					break;

				case 9: //route from all nodes
					if (rq_vec.size() > 1)
					{
						set_console_error();
						std::cout << "[ERROR]: Demasiados parametros para comando \"-ra\". Parametros necesarios: 0\n\n";
						reset_console_color();
						continue;
					}

					bool repeat = true;
					std::string root_dir;
					do
					{
						std::cout << "Directorio > ";
						std::getline(std::cin, root_dir);
						root_dir = Validator::trim(root_dir);
						if (!fs::exists(root_dir))
						{
							set_console_error();
							std::cout << "[ERROR]: Directorio inexistente: \"\"" << root_dir << "\"\n\n";
							reset_console_color();
							continue;
						}
						repeat = false;
					} while (repeat);

					for (const auto n : g.nodes())
					{
						std::string dir = root_dir + "\\" + n->get_label() + ".cfg";
						auto routes = Validator::route_from_node(n);
						Validator::save_to_file(dir, routes, "ip route ");
					}

					break;
				}
				continue;
			}

			set_console_warning();
			std::cout << "[INFO]: No existe el comando \"" << rq_vec[0] << "\".\n";
			reset_console_color();
			continue;
		}
	} while (!exit);
}