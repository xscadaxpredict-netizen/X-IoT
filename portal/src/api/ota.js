import { apiUpload } from './client.js';

export async function uploadFirmware(file, onProgress) {
  const fd = new FormData();
  fd.append('firmware', file);
  return apiUpload('/api/update', fd, onProgress);
}
