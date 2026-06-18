# CPP-HTTP-Client

## Overview
A lightweight HTTP client built from scratch using BSD sockets, demonstrating low-level network programming in C++.

## Quick Start (1-Click Build)

```bash
git clone https://github.com/skylerblue333/CPP-HTTP-Client.git
cd CPP-HTTP-Client
mkdir build && cd build
cmake .. && make
./http_client
```

## Features
- Raw BSD socket implementation
- DNS resolution via `gethostbyname`
- HTTP/1.0 GET request construction
