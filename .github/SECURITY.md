# Security Policy

## Supported Versions

Currently, Catastrophe is in active development. Security updates will be applied to the latest version on the `main` branch.

| Version | Supported          |
| ------- | ------------------ |
| main    | :white_check_mark: |
| develop | :white_check_mark: |
| older   | :x:                |

## Reporting a Vulnerability

We take the security of Catastrophe seriously. If you believe you have found a security vulnerability,
please report it to us as described below.

### How to Report

**Please do NOT report security vulnerabilities through public GitHub issues.**

Instead, please report them via one of these methods:

1. **GitHub Security Advisories** (Preferred)
   - Go to the [Security tab](https://github.com/and3rn3t/catastrophe/security/advisories)
   - Click "Report a vulnerability"
   - Fill in the details

2. **Email**
   - Send details to the repository owner
   - Include "SECURITY" in the subject line
   - Provide a detailed description of the vulnerability

### What to Include

Please include the following information in your report:

- **Type of vulnerability** (e.g., code execution, privilege escalation, etc.)
- **Full paths of affected source files**
- **Location of the affected code** (tag/branch/commit or direct URL)
- **Step-by-step instructions to reproduce the issue**
- **Proof-of-concept or exploit code** (if possible)
- **Impact of the vulnerability** (what an attacker could do)
- **Suggested fix** (if you have one)

### What to Expect

- **Acknowledgment**: We will acknowledge receipt of your vulnerability report within 48 hours
- **Updates**: We will send you updates on the progress of fixing the vulnerability
- **Resolution**: We aim to resolve critical vulnerabilities within 30 days
- **Credit**: With your permission, we will credit you in the security advisory and CHANGELOG

### Security Update Process

1. The vulnerability is reported privately
2. We confirm the vulnerability and determine its impact
3. We develop and test a fix
4. We release a patch and publish a security advisory
5. We credit the reporter (if they wish to be credited)

## Security Best Practices for Contributors

When contributing to Catastrophe, please follow these security best practices:

### Code Security

- Never commit credentials, API keys, or secrets to the repository
- Validate all user inputs in your code
- Use Unreal Engine's built-in security features
- Follow secure coding practices from Unreal Engine documentation

### Dependencies

- Keep Unreal Engine and plugins up to date
- Review dependencies for known vulnerabilities
- Only use trusted third-party plugins and assets

### Access Control

- Use strong passwords and 2FA on your GitHub account
- Don't share credentials or access tokens
- Be cautious with file permissions

### Testing

- Test your code for potential security issues
- Consider edge cases and error conditions
- Report any security concerns you discover

## Security Resources

- [Unreal Engine Security Best Practices](https://docs.unrealengine.com/5.3/en-US/security-in-unreal-engine/)
- [GitHub Security Features](https://docs.github.com/en/code-security)
- [OWASP Top 10](https://owasp.org/www-project-top-ten/)

## Scope

This security policy applies to:

- The Catastrophe game source code
- Build scripts and tools
- Documentation and examples
- GitHub Actions workflows

This policy does NOT apply to:

- Third-party Unreal Engine plugins (report to their maintainers)
- Unreal Engine itself (report to Epic Games)
- Third-party assets and content

## Questions?

If you have questions about this security policy, please open a discussion in the repository or contact the maintainers.

---

**Thank you for helping keep Catastrophe and its users safe!** 🔒
