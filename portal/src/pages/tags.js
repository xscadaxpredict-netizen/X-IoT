import { icons } from '../utils/icons.js';
import { sectionCard } from '../components/form.js';
import { showToast } from '../components/toast.js';
import { apiUpload } from '../api/client.js';

export const id = 'tags';
export const title = 'Tag Registry';
export const description = 'Upload the tag configuration file defining Modbus register mappings.';
export const showActions = false;

let selectedFile = null;

export function render() {
  return sectionCard(icons.upload, 'File Upload', `
    <div class="upload-area" id="upload-zone">
      ${icons.cloudUpload}
      <div class="upload-label">Drag and drop <strong>tag_config.json</strong> here, or click to browse</div>
      <div class="selected-file" id="file-name"></div>
      <input type="file" id="file-input" accept=".json">
    </div>
    <div style="margin-top:12px">
      <button class="btn btn-outline" id="btn-upload-tags">${icons.upload} Upload Tag Registry</button>
    </div>
  `);
}

export function init() {
  const zone = document.getElementById('upload-zone');
  const input = document.getElementById('file-input');
  const nameEl = document.getElementById('file-name');

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

  document.getElementById('btn-upload-tags').addEventListener('click', async () => {
    if (!selectedFile) { showToast('Select a tag_config.json file first', 'warning'); return; }
    const btn = document.getElementById('btn-upload-tags');
    btn.disabled = true; btn.textContent = 'Uploading...';
    const fd = new FormData(); fd.append('file', selectedFile);
    try {
      const res = await apiUpload('/api/upload-tags', fd);
      if (res.status === 'ok') {
        showToast('Tag registry uploaded successfully', 'success');
        selectedFile = null; input.value = ''; nameEl.style.display = 'none';
      } else { showToast(res.message || 'Upload failed', 'error'); }
    } catch (e) { showToast('Upload connection error', 'error'); }
    finally {
      btn.disabled = false;
      btn.innerHTML = `${icons.upload} Upload Tag Registry`;
    }
  });
}

export function setData() {}
export function getData() { return {}; }
