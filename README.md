# Webserv

A high-performance HTTP server developed in C++98. This project involves writing an actual web server capable of hosting a website, handling multiple clients simultaneously, and executing CGI scripts.

---

### 💡 Key Features
* **I/O Multiplexing:** Uses `select()`, `poll()`, or `epoll()` to manage multiple non-blocking client connections efficiently.
* **HTTP/1.1 Protocol:** Supports `GET`, `POST`, and `DELETE` methods, including headers, status codes, and chunked requests.
* **Custom Configuration Parsing:** Implements a Nginx-like configuration file parser to define ports, server names, error pages, and routes.
* **CGI Support:** Capable of executing server-side scripts (like Python or PHP) by managing pipes and environment variables.
* **Resilience:** Built to be non-blocking and stress-tested to ensure it never crashes, even under heavy load.

---

### 🛠️ Compilation & Usage

This project is built using **C++98** to ensure a deep understanding of standard library fundamentals and manual memory management.

| Rule | Description |
| :--- | :--- |
| `make` | Compiles the `Webserv` binary. |
| `make clean` | Removes object files and the `obj/` directory. |
| `make fclean` | Full cleanup including the binary and temporary files. |
| `make re` | Performs a clean re-compilation. |

**To run the server:**
Pass a configuration file as the primary argument:
```bash
./Webserv path/to/config_file.conf
