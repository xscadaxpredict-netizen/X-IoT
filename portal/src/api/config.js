import { apiGet, apiPost } from './client.js';

export async function loadConfig() {
  return apiGet('/api/config');
}

export async function saveConfig(config) {
  return apiPost('/api/config', config);
}
