# Contributing to Speech Recorder

Thank you for considering contributing to Speech Recorder! 🎉

## Ways to Contribute

### 1. Report Bugs 🐛
- Check if the bug was already reported in Issues
- Use the bug report template
- Include system info, steps to reproduce, and logs

### 2. Suggest Features 💡
- Check if it's already suggested
- Describe use case and benefit
- Consider implementation complexity

### 3. Improve Documentation 📚
- Fix typos or unclear sections
- Add examples or tutorials
- Translate to other languages

### 4. Submit Code 💻
- Follow our coding standards
- Add tests if applicable
- Update documentation

---

## Development Setup

```bash
# Clone the repo
git clone https://github.com/your-repo/speech-recorder.git
cd speech-recorder

# Build debug version
mkdir build-debug && cd build-debug
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)

# Run tests (if available)
ctest
```

---

## Coding Standards

### C++ Style
- Use C++17 features
- Follow Qt naming conventions
- CamelCase for classes, camelCase for variables
- 4-space indentation (no tabs)

### Example:
```cpp
class MyClass : public QObject {
    Q_OBJECT

public:
    explicit MyClass(QWidget* parent = nullptr);
    
signals:
    void somethingHappened();
    
private slots:
    void onButtonClicked();
    
private:
    QString m_memberVariable;
};
```

### Qt Conventions
- Use `Q_OBJECT` macro for classes with signals/slots
- Prefer `QSharedPointer` over raw pointers
- Use Qt string methods (`QString`, not `std::string`)
- Connect signals/slots with new syntax when possible

---

## Pull Request Process

1. **Fork and Branch**
   ```bash
   git checkout -b feature/my-awesome-feature
   ```

2. **Make Changes**
   - Write clear commit messages
   - Keep commits atomic and focused

3. **Test Your Changes**
   ```bash
   ./build/speech-recorder
   ```

4. **Update Documentation**
   - README.md if user-facing
   - Code comments if complex

5. **Submit PR**
   - Describe what and why
   - Link related issues
   - Add screenshots if UI changes

6. **Code Review**
   - Address feedback
   - Be patient and respectful

---

## Commit Message Format

```
[Type] Brief summary (50 chars max)

Detailed explanation if needed (wrap at 72 chars).
Reference issues like #123.

Type: feat, fix, docs, style, refactor, test, chore
```

Examples:
```
[feat] Add Vosk Large model support

Implements download and loading of Vosk Large model.
Resolves #42.

[fix] Crash when microphone disconnected

Added error handling for PulseAudio device removal.
Fixes #37.

[docs] Update BUILDING.md with Fedora instructions
```

---

## Testing

### Manual Testing Checklist
- [ ] Record 30 seconds of speech
- [ ] Test all export formats (TXT, DOCX, PDF)
- [ ] Switch between models
- [ ] Download a model
- [ ] Change settings
- [ ] Test on fresh install (VM recommended)

### Automated Tests (TODO)
```bash
cd build
ctest --verbose
```

---

## Areas We Need Help

### High Priority
- [ ] Windows/macOS ports
- [ ] Real-time streaming transcription
- [ ] Automated tests
- [ ] GPU acceleration (CUDA/ROCm)

### Medium Priority
- [ ] Multiple language UI translations
- [ ] Plugin system
- [ ] Dark/Light theme toggle without restart
- [ ] Audio effects (noise reduction, etc.)

### Low Priority
- [ ] Custom model training integration
- [ ] Speech synthesis (TTS)
- [ ] Cloud backup integration

---

## Code of Conduct

### Be Respectful
- Constructive criticism only
- No harassment or discrimination
- Assume good intentions

### Be Collaborative
- Help newcomers
- Share knowledge
- Give credit where due

### Be Professional
- Stay on topic
- Avoid politics/religion
- Keep it productive

---

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

## Questions?

- **Email:** dev@sparklylabz.com
- **Issues:** GitHub Issues
- **Chat:** (Discord/Matrix coming soon)

---

**Thank you for making Speech Recorder better! ♥**

*Made by SparklyLabz - https://sparklylabz.com*

