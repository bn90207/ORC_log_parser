# ORC_log_parser

功能簡介: (1)由system log(dxpcon.log)篩選出所有已生產產品使用的對位參數。(2)依system log內紀錄生產時間與alignment settings(measure.ini)備存時間，挑選正確的備存檔。(3)合併system log與alignment settings內紀錄參數至單一表格(.csv)，並提供對位結果。

## 1.ORC曝光機 product log產生流程
### 1-1.Product log 製作流程圖
![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/ORC%20exposurer.png?raw=true) 

很幸運地，與Fuji曝光機不同，ORC曝光機提供了所有生產參數的完整履歷。但不幸的是，ORC提供的生產履歷並未統一在同一份文件內；更不幸地，整理成良好格式的alignment log和product log都遺漏了太多項目，導致我們必須閱讀完整但龐雜的system log。

### 1-2.各模組紀錄參數
![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/ORC%20module%20members.png?raw=true)

先來看一下各模組紀錄的參數。由上圖可以看到alignment log和product log幾乎遺漏了所有job settings、alignment settings、light settings內設定的項目。另一方面system log則幾乎完整記錄了所有參數；除了alignment settings以外。還好system log至少提供了線索: alignment settings的檔名。因此蒐集所有system log與alignment settings的備存檔即可找出所有已生產產品使用的對位參數。

![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/log_size.png?raw=true)

閱讀system log最大的困難在於其中包含太多無關的資訊。如上圖，在2017/2/6 05:01:55至2017/2/6 12:41:11期間，system log內紀錄的資料多達37萬行，但經過程式篩選出的生產數量僅有1054筆。這也是需要由程式來取代人員閱讀的主因。

## 2.Collector
### 2-1.Collector 執行流程圖
![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/collector_flowchart.png?raw=true)

儘管collector看似複雜，但一言以蔽之就是逐行地做關鍵字搜尋。因為多個模組不斷在system log內插入執行的結果，所以我們必須不斷執行關鍵字檢查，確認目前的區塊屬於哪個模組，並加入在這區塊內要搜尋的目標。底下簡介各區塊的內容。

### 2-2.System Log組成
#### 2-2-1.Job Name
![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/jobname_in_log.png?raw=true)

每次曝光機啟動均會重新載入所有參數，在system log內的第一行即由"作業名稱"開頭。因此每次閱讀system log都應該先找出關鍵字"作業名稱"，取得Job name。直到遇到下一個"作業名稱"前所紀錄的資訊，均屬於此Job name。

#### 2-2-2.Sequence Counter
![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/seqcounter.png?raw=true)

每開始一次曝光前，sequence counter會先設定曝光使用的批號與流水號。當找到關鍵字"seqcounter.txt"即代表進入sequence counter的區塊。在這區塊內可以搜尋"$LOT"和"$SEQUENCE"取得本次曝光的lot number與sequence number。

*注意在"seqcounter.txt"的左方有著"FS_left"或"FS_right"的字串，代表資料屬於左/右側曝光機，蒐集時應做區隔。

#### 2-2-3.Rcipe Parameters
![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/recipe_parameters.png?raw=true)

當找到"Recipe Parameters"關鍵字時，代表接下來曝光機會載入一連串參數設定，在這個區塊我們能找到light settings、alignment settings' filename。

*注意在"Recipe Parameters"的左方有著"DI-right"或"DI-left"的字串，代表資料屬於左/右側曝光機，蒐集時應做區隔。

##### 2-2-3-1.Light Settings
![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/light%20settings.png?raw=true)

在system log內，IR/Yellow/Green光源強度分別由"AlignRingLight/AlignVerLight/AlignRingLight2"字串代表。由上圖中可看到相同的字串重複四次，分別代表編號0-3的CCD。在下方"LightPosition"則代表照明高度。

##### 2-2-3-2.Alignment Settings' Filename
![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/alignment_settings_filename.png?raw=true)

在system log內，alignment settings的檔名由"MeasIniFile"代表，搜尋此關鍵字可得曝光時使用的檔案名稱。

*alignment settings的檔名會於備存時遭修改，要得到正確的檔名請參考下節說明。

#### 2-2-4.Mark Info
![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/mark%20info.png?raw=true)

接下來我們要找到job settings內剩餘的項目，也就是靶點樣式。先搜尋"mark info."，在當行前方會有著left/right的字串，代表緊接著搜尋到的靶點樣式是屬於左側還是右側機台。確認後在搜尋"DecideMarkPosition"。

![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/mark_type.png?raw=true)

在"DecideMarkPosition"的下方有著Mark[0, 0]的參數名，第一個等號右方為mark type(RingMark)；而第二個等號右方連續的數字分別代表: collection diameter(1.3)、polarity(0)、mark number(12)與circle diameter(0.26)

*在不同型號的曝光機中靶點樣式的參數名可能是Mark[0, 0]或Mark[0]。在程式中實際上是搜尋"Mark\["字串。

#### 2-2-5.plcAlignSock_OnAlignMeasEvent
##### 2-2-5-1.Enter plcAlignSock_OnAlignMeasEvent
![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/enter%20align%20event.png?raw=true)

儘管找出了上述的參數設定，並不保證曝光機這次啟動曾經有曝光/對位過。為了確認是否有對位事件發生，可以搜尋"Enter plcAlignSock_OnAlignMeasEvent"。當出現此關鍵字，代表已經有對位事件發生。進入對位事件時，我們有以下幾種情況要注意。

*"Enter plcAlignSock_OnAlignMeasEvent"後有著"right"或"left"的字串，代表對位事件發生在左/右側曝光機，蒐集時應做區隔。

###### 2-2-5-1-1.Camscale vs. Mark
![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/camscale_vs_mark.png?raw=true)

每次曝光對位時，對位的對象有兩種可能:
1. Camscale: 曝光機用來校正檯面原點的光學點。如上圖左方，當對位事件內包含CamScale[...]的字串時，表示此次對位對象為光學點，應略過。
2. Mark: 產品上的靶點(對位點)。如上圖右方，當對位事件內包含Mark[...]的字串時，就表示對位靶點，應該蒐集此次對位結果。

###### 2-2-5-1-2 Mark not Found
![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/mark%20not%20found.png?raw=true)

對位成功與否可藉由搜尋"Mark NotFound"關鍵字確認。當出現時代表對位失敗；反之，對位成功。

##### 2-2-5-2.Exit plcAlignSock_OnAlignMeasEvent
![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/exit_align_event.png?raw=true)

通常每次對位事件包含一組"Enter plcAlignSock_OnAlignMeasEvent"與"Exit plcAlignSock_OnAlignMeasEvent"字串，藉由檢查是否到達"Exit plcAlignSock_OnAlignMeasEvent"，我們應可確認對位事件結束，並輸出對位參數與對位結果。然而存在一種例外情形。

曝光機每次對位會同時對位2個靶點。假設產品上有16個靶點，曝光機總共會對位8次。底下分成第1-7次，與第8次對位結束時，system log內留下的紀錄討論。

###### 2-2-5-2-1.第1-7次對位結果(尚未完成所有對位點對位時)
![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/normal_exit_align_event.png?raw=true)

此時對位事件包含一組"Enter plcAlignSock_OnAlignMeasEvent"與"Exit plcAlignSock_OnAlignMeasEvent"字串，藉由檢查是否到達"Exit plcAlignSock_OnAlignMeasEvent"，我們可確認對位事件結束，並輸出結果。

###### 2-2-5-2-2.第8次對位結果(已完成所有對位點對位時)
![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/abnormal_exit_align_event.png?raw=true)

如上圖，此時對位事件僅包含"Enter plcAlignSock_OnAlignMeasEvent"，因此無法藉由檢查"Exit plcAlignSock_OnAlignMeasEvent"，來確認對位事件是否結束。

在程式內解決的辦法是，如果連續找到兩個"Enter plcAlignSock_OnAlignMeasEvent"，中途未找到"Exit plcAlignSock_OnAlignMeas，則在進入第二個"Enter plcAlignSock_OnAlignMeasEvent"前先將第一次的對位結果輸出。

### 2-3.Alignment Settings正確檔名
![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/Alignment%20settings%20timeline.png?raw=true)

如2-2-3-2所述，system log內所記錄的alignment settings檔名僅僅是生產當下時選定的檔名。相同的檔案在生產後可以被人員修改並存成相同檔名，此時曝光機會自動產生一個備存檔，檔名格式為: 原始檔名_修改時間.backup。

以上圖為例。在1/1時，alignment settings的檔名為"measure.ini"。在1/2時，因原檔案內容被修改，此時1/1使用的參數被備存到"measure_170102.backup"的檔案內。同樣地，在1/3檔案內容又被修改一次，此時1/2使用的參數則備存到"measure_170103.backup"內。

儘管事後我們檢查從1/1到1/3，system log內紀錄的檔名均為"measure.ini"，但實際上1/1生產使用的參數記錄在"measure_170102.backup"、1/2使用的參數記錄在"measure_170103.backup"、1/3才是使用"measure.ini"內的設定值。

由備存的機制可以知道，正確的檔名是由生產紀錄時間開始算起，修改時間最接近生產時間的備存檔。

![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/alignment_settings_backup.png?raw=true)

在示意圖中，為減少文字有稍作簡化。真正備存檔上的修改時間如上圖，包含各兩碼的西元年月日時分秒。假設今天在system log上找到的檔案名稱為Z(M)1.ini、生產時間為170222235900，依上圖我們應讀取Z(M)1_170223005757.backup。


