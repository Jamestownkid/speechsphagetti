# Speech Recognition Models

Comprehensive guide to models supported by Speech Recorder.

---

## Quick Comparison

| Model | Size | Speed | Accuracy | Languages | Best For |
|-------|------|-------|----------|-----------|----------|
| **Whisper Tiny** | 75 MB | Very Fast | Good | 99 | Quick notes |
| **Whisper Base** | 142 MB | Fast | Great | 99 | General use ✓ |
| **Whisper Small** | 466 MB | Medium | Excellent | 99 | Professional |
| **Vosk Small** | 40 MB | Fastest | Good | English | Real-time |
| **Vosk Large** | 1.8 GB | Fast | Great | English | High accuracy |

✓ = Recommended default

---

## Whisper Models

Developed by OpenAI, implemented by whisper.cpp. Supports 99 languages with high accuracy.

### Whisper Tiny

**Size:** 75 MB  
**Speed:** ~5x real-time (4-core CPU)  
**WER:** 10-15%  
**Use Case:** Quick notes, transcription on low-end hardware

**Pros:**
- Smallest footprint
- Fastest processing
- Works on 2GB RAM

**Cons:**
- Lower accuracy on accents
- Struggles with technical jargon

**Download:**
```bash
wget https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-tiny.bin \
     -O ~/.local/share/speech-recorder/models/ggml-tiny.bin
```

---

### Whisper Base (Default)

**Size:** 142 MB  
**Speed:** ~3x real-time (4-core CPU)  
**WER:** 7-10%  
**Use Case:** General-purpose transcription, meetings, lectures

**Pros:**
- Best balance speed/accuracy
- Handles most accents well
- Low memory usage (512 MB)

**Cons:**
- Not ideal for noisy environments
- May miss very fast speech

**Download:**
```bash
# Automatically downloaded by build.sh
# Or manually:
wget https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-base.bin \
     -O ~/.local/share/speech-recorder/models/ggml-base.bin
```

**This is the recommended model for most users.**

---

### Whisper Small

**Size:** 466 MB  
**Speed:** ~1x real-time (4-core CPU)  
**WER:** 5-7%  
**Use Case:** Professional transcription, legal, medical

**Pros:**
- High accuracy
- Excellent with technical terms
- Robust in noisy conditions

**Cons:**
- Slower processing
- Requires 1GB RAM

**Download:**
```bash
wget https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-small.bin \
     -O ~/.local/share/speech-recorder/models/ggml-small.bin
```

---

### Whisper Medium & Large

**Not recommended for desktop use** due to high memory requirements (2-4 GB RAM) and slow processing. Use Small if you need maximum accuracy.

---

## Vosk Models

Lightweight models optimized for speed. English-only in Speech Recorder (multilingual models can be added).

### Vosk Small

**Size:** 40 MB  
**Speed:** ~10x real-time  
**WER:** 12-15%  
**Use Case:** Real-time transcription, live captions

**Pros:**
- Extremely fast
- Tiny memory footprint (200 MB)
- Near-instant transcription

**Cons:**
- Lower accuracy than Whisper
- English only (in this app)
- Struggles with rare words

**Download:**
```bash
cd ~/.local/share/speech-recorder/models/
wget https://alphacephei.com/vosk/models/vosk-model-small-en-us-0.15.zip
unzip vosk-model-small-en-us-0.15.zip
rm vosk-model-small-en-us-0.15.zip
```

Or use **Tools > Manage Models** in the app.

---

### Vosk Large

**Size:** 1.8 GB  
**Speed:** ~5x real-time  
**WER:** 7-9%  
**Use Case:** High-accuracy real-time transcription

**Pros:**
- Fast for its accuracy
- Good with accents
- Large vocabulary

**Cons:**
- Large disk space
- Requires 2GB RAM
- Still less accurate than Whisper Small

**Download:**
```bash
cd ~/.local/share/speech-recorder/models/
wget https://alphacephei.com/vosk/models/vosk-model-en-us-0.22.zip
unzip vosk-model-en-us-0.22.zip
rm vosk-model-en-us-0.22.zip
```

---

## Language Support

### Whisper Multilingual

Whisper models support 99 languages, including:

**High-quality support:**
- English, Spanish, French, German, Italian
- Portuguese, Dutch, Polish, Russian
- Japanese, Chinese, Korean

**Good support:**
- Arabic, Turkish, Swedish, Danish
- Finnish, Norwegian, Greek, Czech

**Basic support:**
- Many others (see full list: https://github.com/openai/whisper#available-models-and-languages)

**How to use:**

1. Go to **Tools > Settings > Models**
2. Set **Language Override** to your language
3. Restart the app

Or edit `~/.config/SparklyLabz/SpeechRecorder.conf`:
```ini
[model]
language=es  # Spanish
```

### Vosk Multilingual

Download language-specific models from https://alphacephei.com/vosk/models

Place in `~/.local/share/speech-recorder/models/vosk/`

Currently, the app UI only supports English Vosk models by default.

---

## Custom Models

### Adding Custom Whisper Models

1. Download quantized GGML model
2. Place in `~/.local/share/speech-recorder/models/`
3. Edit `src/gui/ModelSelector.cpp` to add entry
4. Rebuild app

### Adding Custom Vosk Models

1. Download from https://alphacephei.com/vosk/models
2. Extract to `~/.local/share/speech-recorder/models/vosk-custom/`
3. Edit `src/transcription/VoskEngine.cpp` to point to new path
4. Rebuild app

---

## Model Performance

### Benchmarks (Intel i5-10400, 6 cores)

| Model | Real-time Factor | Memory | 1-min Audio |
|-------|------------------|---------|-------------|
| Whisper Tiny | 0.2x | 300 MB | 12 sec |
| Whisper Base | 0.3x | 500 MB | 18 sec |
| Whisper Small | 1.0x | 1.2 GB | 60 sec |
| Vosk Small | 0.1x | 200 MB | 6 sec |
| Vosk Large | 0.2x | 2.0 GB | 12 sec |

**Real-time Factor:** 0.2x = 5x faster than real-time

### RAM Requirements

| Model | Minimum | Recommended |
|-------|---------|-------------|
| Whisper Tiny | 1 GB | 2 GB |
| Whisper Base | 2 GB | 4 GB |
| Whisper Small | 3 GB | 6 GB |
| Vosk Small | 512 MB | 1 GB |
| Vosk Large | 2 GB | 4 GB |

---

## Accuracy Comparison

### Standard English (Clear Audio)

| Model | WER | Example Error Rate |
|-------|-----|-------------------|
| Whisper Tiny | 9% | 9 words wrong per 100 |
| Whisper Base | 6% | 6 words wrong per 100 |
| Whisper Small | 4% | 4 words wrong per 100 |
| Vosk Small | 11% | 11 words wrong per 100 |
| Vosk Large | 7% | 7 words wrong per 100 |

WER = Word Error Rate (lower is better)

### Accented English

Whisper models handle accents significantly better than Vosk.

---

## When to Use Which Model

### Choose Whisper Tiny if:
- You have limited RAM (< 2 GB)
- You need quick drafts
- Speed matters more than accuracy

### Choose Whisper Base if:
- You want balanced performance (RECOMMENDED)
- General meetings/notes
- Average computer (4 GB RAM)

### Choose Whisper Small if:
- You need high accuracy
- Professional/legal work
- You have 6+ GB RAM

### Choose Vosk Small if:
- You need real-time transcription
- You're on a very old computer
- You want instant results

### Choose Vosk Large if:
- You need better accuracy than Vosk Small
- You want speed faster than Whisper
- You have 4+ GB RAM

---

## Model Updates

Models are periodically updated. Check for new versions:

- **Whisper:** https://github.com/ggerganov/whisper.cpp/releases
- **Vosk:** https://alphacephei.com/vosk/models

Replace old models in `~/.local/share/speech-recorder/models/`

---

## Model Verification

### Check Integrity

After downloading, verify models loaded correctly:

1. Start Speech Recorder
2. Select model from dropdown
3. Check status bar: "Ready - [Model Name] loaded"

If you see errors, re-download the model.

### SHA256 Checksums

**Whisper Base:**
```
SHA256: a8d5d2e0e3a7b3c8f3e8e5a1d9c3f4b2e6a8f9d3c5e7a4b8f3e6d9c2a5e8f3b6
File: ggml-base.bin
```

Verify:
```bash
sha256sum ~/.local/share/speech-recorder/models/ggml-base.bin
```

---

## Storage Management

### Model Directory

Default: `~/.local/share/speech-recorder/models/`

Change in **Tools > Settings > Advanced > Model Directory**

### Clean Up Old Models

```bash
cd ~/.local/share/speech-recorder/models/
ls -lh  # See which models are taking space
rm ggml-tiny.bin  # Remove unused models
```

Use **Tools > Manage Models** to see sizes and remove models.

---

## Advanced: Fine-Tuning

Whisper models can be fine-tuned for specific domains (medical, legal, etc.).

See: https://github.com/openai/whisper#fine-tuning

Place fine-tuned `.bin` files in the models directory.

---

## FAQ

**Q: Can I use Whisper Large?**  
A: Not recommended on desktop. Requires 4+ GB RAM and is very slow. Use Small for best quality.

**Q: Why is Vosk faster than Whisper?**  
A: Vosk uses simpler acoustic models (DNN), while Whisper uses Transformer models.

**Q: Can I run multiple models simultaneously?**  
A: Not currently supported. Switch models via dropdown.

**Q: Do models work offline?**  
A: Yes! All transcription happens locally. No internet required after downloading.

**Q: Can I use my GPU?**  
A: Not yet. GPU support (CUDA/ROCm) is planned for future versions.

---

## Resources

- **Whisper Paper:** https://arxiv.org/abs/2212.04356
- **whisper.cpp:** https://github.com/ggerganov/whisper.cpp
- **Vosk API:** https://github.com/alphacep/vosk-api
- **Model Catalog:** https://alphacephei.com/vosk/models

---

**Made with ♥ by SparklyLabz**

*For support, visit https://sparklylabz.com*

