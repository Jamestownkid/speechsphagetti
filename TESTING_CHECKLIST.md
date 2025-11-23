# 🧪 Testing Checklist - Fresh GitHub Download

## ✅ All 3 Critical Bugs FIXED!

Run this complete test to verify everything works:

---

## 📥 Step 1: Fresh Download

1. **Go to GitHub**: https://github.com/Jamestownkid/speechsphagetti
2. **Click**: Green "Code" button → "Download ZIP"
3. **Extract** to a fresh location (e.g., `~/Downloads/test/`)
4. **Open** the extracted folder

---

## 🐧 Step 2: Run Installer

1. **Double-click**: `CLICK_ME_IF_YOUR_ON_LINUX.sh`
2. **Enter your password** when prompted
3. **Wait** for installation (3-5 minutes)
4. **Look for**:
   ```
   ✅ whisper.cpp cloned
   ✅ Whisper Base model downloaded to /home/USER/.local/share/speech-recorder/models/
   ✅ Installed to /usr/bin/speech-recorder
   ✅ Desktop entry created
   ```

---

## 🔍 Step 3: Verify Installation

### Test 1: App in Applications Menu
1. Press **Super key** (Windows key)
2. Type: `speech recorder`
3. **Expected**: ✅ App appears in search results!

### Test 2: Launch from Terminal
```bash
speech-recorder
```
**Expected**: ✅ App launches without errors!

### Test 3: Check Model Location
```bash
ls -lh ~/.local/share/speech-recorder/models/
```
**Expected**: ✅ You see `ggml-base.bin` (142 MB)

---

## 🎤 Step 4: Test App Functionality

### Test 4: App Opens Without Errors
- **Expected**: ✅ No "Model file not found" error
- **Expected**: ✅ Model selector shows "Whisper Base" as selected
- **Expected**: ✅ Record button is enabled

### Test 5: Close and Reopen
1. Close the app
2. Reopen from Applications menu
3. **Expected**: ✅ Still finds models (no error!)
4. **Expected**: ✅ Previous model selection remembered

### Test 6: Model Manager
1. Click: **Tools → Manage Models**
2. **Expected**: ✅ Shows "Whisper Base ✓ Ready"
3. **Expected**: ✅ Shows model directory path
4. **Expected**: ✅ Can download other models
5. **Expected**: ✅ Can remove models

---

## ✅ Success Criteria

All of these should pass:

- ✅ **Build**: Compiles without errors from GitHub ZIP
- ✅ **Install**: App appears in Applications menu
- ✅ **Launch**: Opens without "Model not found" error
- ✅ **Models**: Located in `~/.local/share/speech-recorder/models/`
- ✅ **Persistence**: Models found after close/reopen
- ✅ **Functionality**: Can record and transcribe

---

## 🐛 If Something Fails

### Issue: "whisper.cpp not found" during build
**Solution**: This is fixed! Re-download from GitHub.

### Issue: App not in Applications menu
**Solution**: This is fixed! Re-download from GitHub.

### Issue: "Model file not found" error
**Solution**: This is fixed! Re-download from GitHub.

### Issue: Still seeing errors?
**Check**:
```bash
# 1. Verify model exists
ls ~/.local/share/speech-recorder/models/

# 2. Check app can find it
speech-recorder --version 2>&1 | grep -i model

# 3. Re-run installer
cd ~/Downloads/speechsphagetti-main
./CLICK_ME_IF_YOUR_ON_LINUX.sh
```

---

## 📊 Expected Results

After a **fresh GitHub download and install**:

```
✅ Installer runs successfully
✅ whisper.cpp clones automatically  
✅ Base model downloads to correct location
✅ App installs to /usr/bin/
✅ Desktop entry created
✅ App appears in Applications menu
✅ App launches without errors
✅ Models persist between sessions
✅ Can record audio
✅ Can transcribe speech
✅ Can export to TXT/DOCX/PDF
```

---

## 🎉 Report Back!

After testing, report:
- ✅ All tests passed!
- ❌ Which test failed (with error message)

---

**Made with ♥ by SparklyLabz**

Last updated: After fixing all 3 critical bugs

