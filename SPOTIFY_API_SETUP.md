# Spotify API Integration Setup

## Overview
The Spotify Counter feature integrates with the Spotify Web API to fetch real-time artist statistics including popularity, album count, top tracks, and genres for Jay Thakur.

## Setup Instructions

### 1. Get Spotify Developer Credentials (FREE!)

1. **Go to Spotify Developer Dashboard**
   - Visit: https://developer.spotify.com/dashboard
   - Log in with your Spotify account (or create one if needed)

2. **Create a New App**
   - Click "Create App"
   - Fill in:
     - **App Name**: "Stats Boards" (or whatever you prefer)
     - **App Description**: "LED Matrix display for artist statistics"
     - **Website**: Can be your GitHub repo or any URL
     - **Redirect URI**: `http://localhost:8080/callback` (for testing)
   - Check the boxes for terms and conditions
   - Click "Save"

3. **Get Your Credentials**
   - You'll see your **Client ID** and **Client Secret**
   - These are completely free to use!

### 2. Set Environment Variables

Set the Spotify credentials as environment variables:

```bash
export SPOTIFY_CLIENT_ID="your_client_id_here"
export SPOTIFY_CLIENT_SECRET="your_client_secret_here"
```

To make it permanent, add to your `~/.bashrc` or `~/.zshrc`:
```bash
echo 'export SPOTIFY_CLIENT_ID="your_client_id_here"' >> ~/.bashrc
echo 'export SPOTIFY_CLIENT_SECRET="your_client_secret_here"' >> ~/.bashrc
source ~/.bashrc
```

### 3. Build and Run

```bash
cd "/Users/jay_here/Stats Boards"
make clean
make
sudo ./led_matrix_apps
```

### 4. Use the Spotify Feature

1. **Start the app** - you'll see the main menu
2. **Type `spotify`** and press Enter
3. **The app will automatically load Jay Thakur's data**

## Available Metrics

The Spotify integration displays:

- **Artist Name**: "Jay Thakur"
- **Popularity**: "Popularity: 45" (0-100 scale)
- **Album Count**: "6 Albums"
- **Top Track**: "Top: Creed"
- **Genres**: "Electronic, Progressive House"

## Keyboard Controls

- **Press 'r'**: Refresh current data
- **Type 'back'**: Return to main menu

## Display States

**Loading State:**
```
Loading...
```

**Success State (rotates through):**
```
Jay Thakur
Popularity: 45
6 Albums
Top: Creed
Electronic
```

**Error States:**
```
Error!
API credentials not configured
```
or
```
Error!
Artist not found
```

## API Limitations

**✅ Available:**
- Artist popularity (0-100)
- Album count
- Track count
- Top tracks
- Genres
- Artist name

**❌ NOT Available:**
- Total play counts (Spotify doesn't provide this)
- Follower count (only for playlists, not artists)
- Monthly listeners (limited availability)

## Troubleshooting

**"API credentials not configured"**
- Make sure you set both environment variables correctly
- Run: `echo $SPOTIFY_CLIENT_ID` and `echo $SPOTIFY_CLIENT_SECRET` to check

**"Authentication failed"**
- Double-check your Client ID and Secret
- Make sure they're copied correctly (no extra spaces)

**"Artist not found"**
- The hardcoded artist ID is for Jay Thakur: `6m4ysuZf9XxRhqeujYp5ti`
- This should work if the API is configured correctly

**Build errors**
- Make sure libcurl is installed: `brew install curl` (macOS) or `sudo apt-get install libcurl4-openssl-dev` (Linux)

## Free Tier Limits

- **No cost** for basic API usage
- **Rate limits**: 10,000 requests per hour (more than enough)
- **No credit card required**

## Security Notes

- Never commit credentials to version control
- Use environment variables for credentials
- Monitor API usage in Spotify Developer Dashboard

The Spotify integration uses OAuth2 Client Credentials flow for app-only access, which is perfect for displaying public artist data!
