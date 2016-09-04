class CreateProcessings < ActiveRecord::Migration
  def change
    create_table :processings do |t|
      t.boolean :disp_enable
      t.boolean :var_enable
      t.boolean :median_enable
      t.boolean :wat_ime_enable
      t.boolean :wat_opencv_enable
      t.boolean :merge_enable
      t.integer :dispT
      t.integer :dispJ
      t.integer :varT
      t.integer :varJ
      t.integer :medJ
      t.integer :watT
      t.integer :merT
      t.integer :merJ

      t.timestamps null: false
    end
  end
end
