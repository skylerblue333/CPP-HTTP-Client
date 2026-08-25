# Security

Sky HTTP Client is an engineering-beta native HTTP utility.

## Implemented controls

- Only HTTP and HTTPS URL schemes are accepted.
- Redirects are bounded and restricted to HTTP(S).
- Connection and total request timeouts are enforced.
- Response bodies are capped at 1 MiB.
- CI compiles with strict warnings and AddressSanitizer/UndefinedBehaviorSanitizer instrumentation.

## Important limitations

This utility does not provide an SSRF allowlist. If callers can influence the URL, they can still target loopback, private, link-local, metadata-service, or otherwise sensitive HTTP(S) destinations unless the embedding application or network layer blocks them.

Certificate pinning, mTLS, credential management, proxy policy, DNS-rebinding defenses, retry policy, and secret redaction are not implemented. Do not treat this beta as a complete untrusted-URL fetch service.
