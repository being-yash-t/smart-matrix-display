# YouTube API Integration Setup

## Overview
The YouTube Counter feature now integrates with the YouTube Data API v3 to fetch real-time channel statistics including subscriber count, total views, and video count.

## Setup Instructions

### 1. Get YouTube API Key
1. Go to [Google Cloud Console](https://console.cloud.google.com/)
2. Create a new project or select an existing one
3. Enable the YouTube Data API v3
4. Create credentials (API Key)
5. Restrict the API key for security (recommended)

### 2. Set Environment Variable
Set the `YOUTUBE_API_KEY` environment variable:

```bash
export YOUTUBE_API_KEY="your_api_key_here"
```

To make it permanent, add to your `~/.bashrc` or `~/.zshrc`:
```bash
echo 'export YOUTUBE_API_KEY="your_api_key_here"' >> ~/.bashrc
source ~/.bashrc
```

### 3. Install Dependencies
The project now requires `libcurl` for HTTP requests:

```bash
# On Ubuntu/Debian
sudo apt-get install libcurl4-openssl-dev

# On macOS
brew install curl

# On Raspberry Pi
sudo apt-get update
sudo apt-get install libcurl4-openssl-dev
```

### 4. Build and Run
```bash
make clean
make
sudo ./led_matrix_apps
```

## Usage

### Starting the YouTube App
1. Run the application
2. Select "youtube" from the main menu
3. Enter a YouTube Channel ID when prompted

### Finding Channel IDs
- **Method 1**: Go to the channel page, the ID is in the URL: `youtube.com/channel/UCxxxxxxxxxxxxxxxxxxxxxx`
- **Method 2**: Use the channel's custom URL: `youtube.com/@username` (the app will resolve this)

### Keyboard Controls
- **Type channel ID**: Enter the full channel ID or username
- **Press Enter**: Fetch data for the entered channel
- **Press 'r'**: Refresh current data
- **Type 'back'**: Return to main menu

### Display States
- **Loading**: Shows "Loading..." while fetching data
- **Error**: Shows "Error!" and error message (e.g., "Invalid API key", "Channel not found")
- **Success**: Rotates through:
  - Subscriber count (formatted: 1.2M, 500K, etc.)
  - Total views
  - Video count

## API Quotas
- YouTube Data API has daily quotas
- Each channel stats request costs 1 quota unit
- Default quota: 10,000 units per day
- Monitor usage in Google Cloud Console

## Error Handling
The app handles various error conditions:
- Network connectivity issues
- Invalid API key
- Channel not found
- API quota exceeded
- Malformed responses

All errors are displayed on the LED matrix and logged to console.

## Security Notes
- Never commit API keys to version control
- Use environment variables for API keys
- Consider restricting API keys by IP address
- Monitor API usage regularly
