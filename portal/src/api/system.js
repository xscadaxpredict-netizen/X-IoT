import { apiGet } from './client.js';

export async function getSystemInfo() {
  return apiGet('/api/system');
}
