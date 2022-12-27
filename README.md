# タスクバーに天気を表示するやつ
**自己責任でお願いします．**

Open-Meteoから天気を取ってきて表示するやつです．

![sample](https://user-images.githubusercontent.com/106300228/209650753-2f1b4a70-b3de-487a-b88e-98a0eccc9f95.jpg)

Open-Meteo: https://open-meteo.com/en

## 仕様
Open-Meteoから天気を取ってきてタスクバーに現在の天気，気温，今日の最高気温，最低気温を表示します．
天気の更新は0時から6時間ごとに行われます．天気表示領域をクリックしても天気を更新できます．

## 使い方

### インストール/アンインストール
install.batを実行する．そのあとタスクバーを右クリック→ツールバー→Weather DeskBandをクリック．

<img src="https://user-images.githubusercontent.com/106300228/209650853-bc9efbd9-a27e-4870-b8e9-5c7f430bf06c.jpg" alter="register" width="30%" height="30%" >

アンインストールはuninstall.batを実行してexplorerを再起動する．

### URLの変更(デフォルト:大阪)
　[Open-Meteo](https://open-meteo.com/en/docs#api_form)でURLを作成する．
Hourly Weather Variablesには"Temperature (2 m)"と"Weathercode"，
Daily Weather Variablesには"Maximum Temperature (2 m)"，"Minimum Temperature (2 m)",
"Sunrise","Sunset"を指定する．Timeformatは"Unix timestamp"を指定すること．

　WeatherDeskBand.xml内のURL内のsrc属性の値を作成したURLに置き換える．URL内の"&"は"&amp\;"に置き換える．

<img src="https://user-images.githubusercontent.com/106300228/209653605-997897b9-799c-4608-b0b5-9fb4c37efbd7.jpg" alter="xml" width="60%" height="60%" >

### 天気アイコンの変更
　作成した天気アイコンをimagesに置く．アイコンを変更したいWeatherCodeに対応するIcon要素のfilename属性値を変更する．
## 動作確認環境
Windows10 x64

## ビルド環境
- Visual Studio 2022
- cpprestsdk 2.10.18
- lohmann.json.decomposed 3.11.2

## 問い合わせ先
Mail:u849256c\[at\]ecs.osaka-u.ac.jp

## 参考
以下のサイト，コードを参考にさせていただきました．
- https://learn.microsoft.com/ja-jp/windows/win32/shell/band-objects
- http://eternalwindows.jp/shell/shellex/shellex13.html
- https://github.com/KMConner/TaskbarTweet
