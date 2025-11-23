# GPU Acceleration Support

GPU support for faster transcription is planned but not yet implemented.

## Current Status

- ❌ CUDA support (NVIDIA)
- ❌ ROCm support (AMD)
- ❌ Metal support (Apple Silicon)
- ✅ CPU-only (works on all systems)

## Why Not Included Yet?

GPU support requires:
1. Detecting GPU hardware (NVIDIA/AMD)
2. Compiling whisper.cpp with CUDA/ROCm flags
3. Linking against CUDA/ROCm libraries
4. Handling GPU out-of-memory errors
5. Fallback to CPU if GPU fails

This adds significant complexity and platform-specific dependencies.

## Planned for v1.1.0

We'll add GPU support in the next release with:
- Automatic GPU detection
- Build flags for CUDA/ROCm
- GPU memory checks
- Automatic CPU fallback

## For Advanced Users (Manual GPU Build)

If you want GPU support now:

### NVIDIA (CUDA)

```bash
# Install CUDA
sudo apt install nvidia-cuda-toolkit

# Build whisper.cpp with CUDA
cd external/whisper.cpp
mkdir build && cd build
cmake .. -DGGML_CUDA=ON
make -j$(nproc)
cd ../../..

# Rebuild speech-recorder
rm -rf build
./build.sh
```

### AMD (ROCm)

```bash
# Install ROCm
# See: https://rocm.docs.amd.com/

# Build whisper.cpp with ROCm
cd external/whisper.cpp
mkdir build && cd build
cmake .. -DGGML_HIPBLAS=ON
make -j$(nproc)
cd ../../..

# Rebuild speech-recorder
rm -rf build
./build.sh
```

## Performance Comparison

| Hardware | Whisper Base Speed |
|----------|-------------------|
| CPU (4-core i5) | ~3x real-time |
| CPU (8-core i7) | ~5x real-time |
| NVIDIA RTX 3060 | ~50x real-time |
| NVIDIA RTX 4090 | ~100x real-time |

GPU makes a huge difference for large models!

## Help Wanted

If you have experience with CUDA/ROCm and want to contribute GPU support, please:
1. Fork the repo
2. Add GPU detection code
3. Update CMakeLists.txt with GPU flags
4. Test on your hardware
5. Submit a pull request

See CONTRIBUTING.md for guidelines.

---

**Made with ♥ by SparklyLabz**

