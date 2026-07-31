import { icons } from '../utils/icons.js';
import { sectionCard } from '../components/form.js';
import { showToast } from '../components/toast.js';
import { uploadFirmware } from '../api/ota.js';

export const id = 'ota';
export const title = 'Firmware Update';
export const description = 'Upload a new .bin firmware file for an Over-The-Air update.';
export const showActions = false;

let selectedFile = null;

export function render() {
  return sectionCard(icons.download, 'OTA Update', `
    <div class="upload-area" id="ota-upload-zone">
      ${icons.cloudUpload}
      <div class="upload-label">Drag and drop <strong>firmware.bin</strong> here, or click to browse</div>
      <div class="selected-file" id="ota-file-name"></div>
      <input type="file" id="ota-file-input" accept=".bin">
    </div>
    <div class="progress-bar" id="ota-progress">
      <div class="progress-fill" id="ota-progress-fill"></div>
    </div>
    <div class="progress-text" id="ota-progress-text"></div>
    <div style="margin-top:12px">
      <button class="btn btn-primary" id="btn-upload-ota">${icons.upload} Upload Firmware</button>
    </div>
  `);
}

export function init() {
  const zone = document.getElementById('ota-upload-zone');
  const input = document.getElementById('ota-file-input');
  const nameEl = document.getElementById('ota-file-name');

  zone.addEventListener('click', () => input.click());
  zone.addEventListener('dragover', e => { e.preventDefault(); zone.classList.add('dragover'); });
  zone.addEventListener('dragleave', () => zone.classList.remove('dragover'));
  zone.addEventListener('drop', e => {
    e.preventDefault(); zone.classList.remove('dragover');
    if (e.dataTransfer.files.length) {
      selectedFile = e.dataTransfer.files[0];
      nameEl.textContent = selectedFile.name;
      nameEl.style.display = 'block';
    }
  });
  input.addEventListener('change', () => {
    if (input.files.length) {
      selectedFile = input.files[0];
      nameEl.textContent = selectedFile.name;
      nameEl.style.display = 'block';
    }
  });

  document.getElementById('btn-upload-ota').addEventListener('click', async () => {
    if (!selectedFile) { showToast('Select a firmware .bin file first', 'warning'); return; }
    const btn = document.getElementById('btn-upload-ota');
    const progressBar = document.getElementById('ota-progress');
    const progressFill = document.getElementById('ota-progress-fill');
    const progressText = document.getElementById('ota-progress-text');
    btn.disabled = true; btn.textContent = 'Uploading...';
    progressBar.style.display = 'block';
    progressText.style.display = 'block';
    try {
      const res = await uploadFirmware(selectedFile, (pct) => {
        progressFill.style.width = pct + '%';
        progressText.textContent = `Uploading: ${pct}%`;
      });
      if (res.status === 'ok') {
        progressFill.style.width = '100%';
        progressText.textContent = 'Upload complete. Rebooting...';
        showToast('Firmware updated. Rebooting...', 'success');
        setTimeout(() => window.location.reload(), 5000);
      } else {
        showToast(res.message || 'OTA update failed', 'error');
        btn.disabled = false;
        btn.innerHTML = `${icons.upload} Upload Firmware`;
      }
    } catch (e) {
      showToast('Upload failed', 'error');
      btn.disabled = false;
      btn.innerHTML = `${icons.upload} Upload Firmware`;
    }
  });
}

export function setData() {}
export function getData() { return {}; }
