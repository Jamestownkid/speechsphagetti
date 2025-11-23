# Model Management Guide

## ✅ Delete/Remove Models Feature - ALREADY INCLUDED!

Your app already has a full-featured model manager with **Remove** buttons!

---

## 🗑️ How to Remove/Delete Models

### Step 1: Open Model Manager
```
Tools → Manage Models
```

### Step 2: View Downloaded Models
You'll see a table like this:

```
┌─────────────────┬──────┬───────────┬──────────┬──────────┐
│ Model Name      │ Size │ Status    │ Action   │ Progress │
├─────────────────┼──────┼───────────┼──────────┼──────────┤
│ Whisper Tiny    │ 75MB │ Not Dl    │[Download]│          │
│ Whisper Base    │142MB │ ✓ Ready   │[Remove]  │          │
│ Whisper Small   │466MB │ ✓ Ready   │[Remove]  │          │
│ Vosk Small      │ 40MB │ Not Dl    │[Download]│          │
└─────────────────┴──────┴───────────┴──────────┴──────────┘

Models stored in: /home/admin/.local/share/speech-recorder/models/
2 models downloaded | 608 MB used
```

### Step 3: Click Remove
- Click the **[Remove]** button next to any downloaded model
- Confirm deletion
- Model is deleted from disk
- Space is freed!

---

## 🎯 What the Remove Feature Does

✅ **Deletes model file** from disk  
✅ **Frees up storage space**  
✅ **Shows how much space freed**  
✅ **Updates the model list**  
✅ **Shows new storage total**  
✅ **Can't remove if model is currently loaded** (safe)

---

## 📊 Storage Info

At the bottom of Model Manager, you see:
```
Models stored in: /home/admin/.local/share/speech-recorder/models/
3 models downloaded | 680 MB used
```

After removing a model:
```
2 models downloaded | 142 MB used ✓ Freed 538 MB!
```

---

## 🔄 Download Again

If you remove a model and want it back:
1. Open Model Manager
2. Click **[Download]** next to that model
3. Wait for download
4. Done!

---

## 💡 Smart Tips

### When to Remove Models

**Remove when:**
- ❌ Low on disk space
- ❌ Model too slow for your hardware
- ❌ Don't need that language variant
- ❌ Want to try a different model

**Keep when:**
- ✅ Using regularly
- ✅ Have plenty of storage
- ✅ Need offline access

### Storage-Saving Strategy

1. **Start with Whisper Base** (142 MB) - works great!
2. **Try Tiny** (75 MB) if you want faster
3. **Remove Tiny if Base is better**
4. **Only download Large models if you need max accuracy**
5. **Remove models you don't use**

### Model Size Guide

| Model | Size | Keep if... |
|-------|------|------------|
| **Tiny** | 75 MB | Need speed, limited storage |
| **Base** | 142 MB | Perfect for most users ⭐ |
| **Small** | 466 MB | Need better accuracy |
| **Medium** | 1.5 GB | Professional transcription |
| **Large** | 2.9 GB | Maximum quality needed |

---

## 🔒 Safety Features

The Remove feature is safe:
- ✅ Asks for confirmation before deleting
- ✅ Can't accidentally remove system files (only models folder)
- ✅ Shows exactly what will be deleted
- ✅ Can always re-download
- ✅ Doesn't affect other apps

---

## 📂 Manual Removal (Advanced)

You can also manually delete models:

```bash
# View models
ls ~/.local/share/speech-recorder/models/

# Remove a specific model
rm ~/.local/share/speech-recorder/models/ggml-large-v3.bin

# Remove all models (careful!)
rm -rf ~/.local/share/speech-recorder/models/*.bin

# Check space freed
du -sh ~/.local/share/speech-recorder/
```

---

## 🐛 Troubleshooting

### "Remove button is grayed out"
- Model is currently loaded
- Close app and try again
- Or switch to a different model first

### "Model still shows as downloaded after removal"
- Click the refresh button
- Or close and reopen Model Manager

### "Removed but space not freed"
- Check: `du -sh ~/.local/share/speech-recorder/models/`
- May be in trash - empty trash
- Some file managers move to trash instead of deleting

---

## 🎉 Summary

**Your app ALREADY has model removal built-in!**

Just:
1. Open: Tools → Manage Models
2. Click: **[Remove]** button
3. Confirm: Yes
4. Done: Space freed! 🎉

---

**Made with ♥ by SparklyLabz**

*The Remove feature was included from day one!*

