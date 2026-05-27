# Plan: C++ Build Tools Upgrade — 修正計画

## 目的
`libOne` ソリューションのビルドツールアップグレードに伴うビルド警告（C4244, C4312 等）を解消し、ビルドを安定化させる。

## スコープ
- 修正対象:
  - `C:\Users\Eijo\source\repos\libOne\src\graphic.cpp` に起因するテンプレート変換警告
  - `C:\Users\Eijo\source\repos\libOne\src\stb_image_reader.cpp` のポインタ/整数キャスト警告

- 除外:
  - Visual Studio の内部ヘッダー（`xmemory` 等）自体は編集しない。

## 実行ステップ（概要）
1. 新しいブランチ作成指示を確認（ユーザーがブランチ作成を希望するか確認）。
2. `graphic.cpp` の該当箇所を cppupgrade_read_file_range で読み、引数型を調査して修正案を適用。
3. インクリメンタルビルド (`cppupgrade_build_and_get_issues`) で影響範囲を確認。
4. `stb_image_reader.cpp` を読み、intptr_t/uintptr_t などの明示的なキャストや変数型変更で修正。
5. インクリメンタルビルドで検証。
6. 最終的に `cppupgrade_rebuild_and_get_issues` でフルリビルドして最終確認。

## 検証基準
- 変更後に該当警告が解消され、他の新しいエラーが発生していないこと。
- 重要な API や動作の振る舞いを変えないこと（API 変更は最小限）。

## 実行許可
承認をもらえれば自動で修正を開始します。続けてよいですか？