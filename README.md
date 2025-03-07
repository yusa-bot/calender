# CLIカレンダー & 予定管理ツール

## 概要
このツールは、コマンドライン上で予定を管理できるCLIアプリケーションです。  
カレンダー表示・予定の追加/削除/一覧表示が可能です。  

## インストール&使い方
-1 clone
-2 コンパイル
↓
## 主な機能
**カレンダーの表示**
```bash
./a.out -d 2025-03
```

**予定を追加**
```bash
./a.out -a "友達とカフェ" -d 2025-03-10
```

**予定を削除**
```bash
./a.out -d 2025-03-10 -delete
```

**予定の表示**
```bash
./a.out -d 2025-03-10 -list
```

**月の予定を一覧表示**
```bash
./a.out -d 2025-03 -list
```

## 使用技術
- C言語（システムプログラムに最適化）
- ファイル操作（CSV形式）（予定データを schedule.csv に保存）
- CLIインターフェース（直感的なコマンド設計）

作成者: yusa-bot
[GitHub](https://github.com/yusa-bot)