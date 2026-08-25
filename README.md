# Sky HTTP Client

**Status: engineering beta.** A small C++17/libcurl command-line HTTP GET client with explicit protocol, timeout, redirect, and response-size boundaries.

## Implemented

- Real HTTP and HTTPS GET requests through libcurl.
- `http://` and `https://` are the only accepted URL schemes.
- Redirects are limited to three and remain restricted to HTTP(S).
- 2-second connection timeout and 5-second total request timeout.
- 1 MiB response-body ceiling.
- Stable user agent: `sky-http-client/0.1`.
- Exit code `0` for HTTP 2xx/3xx responses, `4` for other HTTP status codes, `1` for transport/client failures, and `2` for CLI usage errors.
- CMake C++17 build with warnings treated as errors.
- Deterministic CI against a local HTTP fixture plus ASan/UBSan compilation.

## Build and run

Requires CMake, a C++17 compiler, and libcurl development headers.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/app https://example.com/
```

## Scope limitations

This is a focused GET client, not a browser or general API SDK. It does **not** implement POST/PUT bodies, authentication helpers, cookie persistence, caching, HTTP/3 policy, certificate pinning, proxy configuration, retries, streaming-to-disk, concurrency, SSRF allowlists, or production service discovery.

The scheme restriction alone is not an SSRF defense. Applications accepting untrusted destination URLs must add hostname/IP/network allowlisting and DNS-rebinding protections appropriate to their deployment.

## SKYCOIN4444 integration

Use this as a bounded native HTTP utility only where destination trust is established externally. Higher-level service authentication, retry policy, observability, and network egress controls remain integration responsibilities.
