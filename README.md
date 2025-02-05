# Analyse-Apache-logs

**Analog** is a command-line tool designed to analyze Apache log files for a given server. This program provides insightful statistics about the server's activity and generates a graphical representation in the form of a `.dot` file, which can be visualized using tools like Graphviz.

## Features

- **Log Analysis**: Extracts and processes key information from Apache log files, such as request counts, response codes, and more.
- **Statistics**: Provides detailed statistics about server usage and activity patterns.
- **Graph Output**: Creates a `.dot` file for visualizing server interactions and data flow.

## Quick Start

To get started with **Analyse-Apache-Logs**, follow these steps:

1. Clone the repository:
    ```bash
    git clone https://github.com/gri-sha/Analyse-Apache-logs.git
    ```

2. Build the project:
    ```bash
    make
    ```

3. Run the program:
    ```bash
    ./analog <file.log> -s <https://server.name.com/>
    ```

This will generate top 10 visited resources in terminal.

## Flags

- `-e` : exclude every resourse except .html in the domain of provided server
- `-g <graph-name.dot>` : generate graph in <graph-name.dot> file
- `-t <hour>` : consider only visits in interval [h, h+1]
- `-s <server>` : change default server name 

## Prerequisites

- Apache log files accessible on your system.
- [Graphviz](https://graphviz.org/) (optional, for rendering the `.dot` file).

## Example

```bash
analog anonyme.log -e -g
```
## Notice that

- Default server: `http://intranet-if.insa-lyon.fr/`