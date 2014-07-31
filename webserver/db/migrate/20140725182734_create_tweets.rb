class CreateTweets < ActiveRecord::Migration
  def change
    create_table :tweets do |t|
      t.string :username, null: false
      t.string :message, null: false
      t.string :tweet_id, null: false
      t.text :profile_image
      t.timestamps
    end

    add_index :tweets, [:tweet_id], unique: true
  end
end
